#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include "objloader.hpp"
#include "../skeleton.h"
#include "../node.h"
#include "../object3d.h"
#include "../mesh.h"


using namespace std;
// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
	){
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			vertex /=50; 
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);

	}
	fclose(file);
	return true;
}



#include <fbxsdk.h>

void DisplayTextureInfo(FbxTexture* pTexture, int pBlendMode)
{
    FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(pTexture);
    FbxProceduralTexture *lProceduralTexture = FbxCast<FbxProceduralTexture>(pTexture);
    std::cout << "            Name: \""<< pTexture->GetName()<< "\""<< std::endl;
    if (lFileTexture)
    {
        std::cout << "            Type: File Texture"<< std::endl;
        std::cout << "            File Name: \""<< lFileTexture->GetFileName() << "\""<< std::endl;
    }
    else if (lProceduralTexture)
    {
        std::cout << "            Type: Procedural Texture";
    }
    std::cout << "            Scale U: "<< pTexture->GetScaleU()<< std::endl;
    std::cout << "            Scale V: "<< pTexture->GetScaleV()<< std::endl;
    std::cout << "            Translation U: "<< pTexture->GetTranslationU()<< std::endl;
    std::cout << "            Translation V: "<< pTexture->GetTranslationV()<< std::endl;
    std::cout << "            Swap UV: "<< pTexture->GetSwapUV()<< std::endl;
    std::cout << "            Rotation U: "<< pTexture->GetRotationU()<< std::endl;
    std::cout << "            Rotation V: "<< pTexture->GetRotationV()<< std::endl;
    std::cout << "            Rotation W: "<< pTexture->GetRotationW()<< std::endl;
    const char* lAlphaSources[] = { "None", "RGB Intensity", "Black" };
    std::cout << "            Alpha Source: "<< lAlphaSources[pTexture->GetAlphaSource()]<< std::endl;
    std::cout << "            Cropping Left: "<< pTexture->GetCroppingLeft()<< std::endl;
    std::cout << "            Cropping Top: "<< pTexture->GetCroppingTop()<< std::endl;
    std::cout << "            Cropping Right: "<< pTexture->GetCroppingRight()<< std::endl;
    std::cout << "            Cropping Bottom: "<< pTexture->GetCroppingBottom()<< std::endl;
    const char* lMappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical", 
        "Box", "Face", "UV", "Environment" };
    std::cout << "            Mapping Type: "<< lMappingTypes[pTexture->GetMappingType()]<< std::endl;
    if (pTexture->GetMappingType() == FbxTexture::ePlanar)
    {
        const char* lPlanarMappingNormals[] = { "X", "Y", "Z" };
        std::cout << "            Planar Mapping Normal: "<< lPlanarMappingNormals[pTexture->GetPlanarMappingNormal()]<< std::endl;
    }
    const char* lBlendModes[]   = { "Translucent", "Additive", "Modulate", "Modulate2", "Over", "Normal", "Dissolve", "Darken", "ColorBurn", "LinearBurn",
                                    "DarkerColor", "Lighten", "Screen", "ColorDodge", "LinearDodge", "LighterColor", "SoftLight", "HardLight", "VividLight",
                                    "LinearLight", "PinLight", "HardMix", "Difference", "Exclusion", "Substract", "Divide", "Hue", "Saturation", "Color",
                                    "Luminosity", "Overlay"};   
    
    if(pBlendMode >= 0)
        std::cout << "            Blend Mode: "<< lBlendModes[pBlendMode]<< std::endl;
    std::cout << "            Alpha: "<< pTexture->GetDefaultAlpha();
    if (lFileTexture)
    {
        const char* lMaterialUses[] = { "Model Material", "Default Material" };
        std::cout << "            Material Use: "<< lMaterialUses[lFileTexture->GetMaterialUse()]<< std::endl;
    }
    const char* pTextureUses[] = { "Standard", "Shadow Map", "Light Map", 
        "Spherical Reflexion Map", "Sphere Reflexion Map", "Bump Normal Map" };
    std::cout << "            Texture Use: "<< pTextureUses[pTexture->GetTextureUse()]<< std::endl;
    std::cout << ""<< std::endl;                
}
void FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex){
    if( pProperty.IsValid() )
    {
        int lTextureCount = pProperty.GetSrcObjectCount<FbxTexture>();
        for (int j = 0; j < lTextureCount; ++j)
        {
            //Here we have to check if it's layeredtextures, or just textures:
            FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
            if (lLayeredTexture)
            {
                std::cout << "    Layered Texture: "<< j<< std::endl;                
                int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
                for(int k =0; k<lNbTextures; ++k)
                {
                    FbxTexture* lTexture = lLayeredTexture->GetSrcObject<FbxTexture>(k);
                    if(lTexture)
                    {
                        if(pDisplayHeader){                    
                            std::cout << "    Textures connected to Material "<< pMaterialIndex<< std::endl;
                            pDisplayHeader = false;
                        }
                        //NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
                        //Why is that?  because one texture can be shared on different layered textures and might
                        //have different blend modes.
                        FbxLayeredTexture::EBlendMode lBlendMode;
                        lLayeredTexture->GetTextureBlendMode(k, lBlendMode);
                        std::cout << "    Textures for "<< pProperty.GetName()<< std::endl;
                        std::cout << "        Texture "<< k<< std::endl;  
                        DisplayTextureInfo(lTexture, (int) lBlendMode);   
                    }
                }
            }
            else
            {
                //no layered texture simply get on the property
                FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
                if(lTexture)
                {
                    //display connected Material header only at the first time
                    if(pDisplayHeader){                    
                        std::cout << "    Textures connected to Material "<< pMaterialIndex<< std::endl;
                        pDisplayHeader = false;
                    }             
                    std::cout << "    Textures for "<< pProperty.GetName()<< std::endl;
                    std::cout << "        Texture "<< j<< std::endl;  
                    DisplayTextureInfo(lTexture, -1);
                }
            }
        }
    }//end if pProperty
}
void DisplayTexture(FbxGeometry* pGeometry)
{
    int lMaterialIndex;
    FbxProperty lProperty;
    if(pGeometry->GetNode()==NULL)
        return;
    int lNbMat = pGeometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
    std::cout << "  Material Count "<< lNbMat<< std::endl;
    for (lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++){
        FbxSurfaceMaterial *lMaterial = pGeometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(lMaterialIndex);
        bool lDisplayHeader = true;
        //go through all the possible textures
        if(lMaterial){
            int lTextureIndex;
            FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
            {
                lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
                FindAndDisplayTextureInfoByProperty(lProperty, lDisplayHeader, lMaterialIndex); 
            }
        }//end if(lMaterial)
    }// end for lMaterialIndex     
}

void registerTexture(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex, std::map<std::string, std::string>& textures){
    if( pProperty.IsValid() )
    {
        int lTextureCount = pProperty.GetSrcObjectCount<FbxTexture>();
        for (int j = 0; j < lTextureCount; ++j)
        {
	        //no layered texture simply get on the property
	        FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
	        if(lTexture)
	        {          
	            std::string type(pProperty.GetName());
	          	FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(lTexture);

			    if (lFileTexture)
			    {
			    	std::string path = lFileTexture->GetFileName();
			    	int beginIdx = path.find_last_of('/');
			    	if(beginIdx==-1){
			    		beginIdx = path.find_last_of('\\');
			    	}

			    	path = path.substr(beginIdx+1) ;
			    	int pos =path.find(' ');
			    	while(pos>=0){
			    		path.replace(pos,1,"_");
			    		pos =path.find(' ');


			    	}

			        textures.insert(std::make_pair(type,path));
			    }

	        }
            
        }
    }//end if pProperty
}

void getTextures(FbxGeometry* pGeometry,  std::map<std::string, std::string>& textures)
{
    int lMaterialIndex;
    int i=0;
    FbxProperty lProperty;
    if(pGeometry->GetNode()==NULL)
        return;
    int lNbMat = pGeometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
    if(lNbMat==2)
    	i=0;
    for (lMaterialIndex=i; lMaterialIndex < lNbMat; lMaterialIndex++){
        FbxSurfaceMaterial *lMaterial = pGeometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(lMaterialIndex);
        bool lDisplayHeader = true;
        //go through all the possible textures
        if(lMaterial){
            int lTextureIndex;
            FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
            {
                lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
                registerTexture(lProperty, lDisplayHeader, lMaterialIndex, textures); 
            }
        }//end if(lMaterial)
    }// end for lMaterialIndex     
}

// Get the matrix of the given pose
FbxAMatrix GetPoseMatrix(FbxPose* pPose, int pNodeIndex)
{
	FbxAMatrix lPoseMatrix;
	FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);
	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));
	return lPoseMatrix;
}
// Get the geometry offset to a node. It is never inherited by the children.
FbxAMatrix GetGeometry(FbxNode* pNode)
{
	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	return FbxAMatrix(lT, lR, lS);
}

FbxAMatrix GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose, FbxAMatrix* pParentGlobalPosition)
{
	FbxAMatrix lGlobalPosition;
	bool        lPositionFound = false;
	if (pPose)
	{
		int lNodeIndex = pPose->Find(pNode);
		if (lNodeIndex > -1)
		{
            // The bind pose is always a global matrix.
            // If we have a rest pose, we need to check if it is
            // stored in global or local space.
			if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
			{
				lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
			}
			else
			{
                // We have a local matrix, we need to convert it to
                // a global space matrix.
				FbxAMatrix lParentGlobalPosition;
				if (pParentGlobalPosition)
				{
					lParentGlobalPosition = *pParentGlobalPosition;
				}
				else
				{
					if (pNode->GetParent())
					{
						lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose,NULL);
					}
				}
				FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
				lGlobalPosition = lParentGlobalPosition * lLocalPosition;
			}
			lPositionFound = true;
		}
	}
	if (!lPositionFound)
	{
        // There is no pose entry for that node, get the current global position instead.
        // Ideally this would use parent global position and local position to compute the global position.
        // Unfortunately the equation 
        //    lGlobalPosition = pParentGlobalPosition * lLocalPosition
        // does not hold when inheritance type is other than "Parent" (RSrs).
        // To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
		lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
	}
	return lGlobalPosition;
}


/**
 * Return a string-based representation based on the attribute type.
 */
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) { 
	switch(type) { 
		case FbxNodeAttribute::eUnknown: return "unidentified"; 
		case FbxNodeAttribute::eNull: return "null"; 
		case FbxNodeAttribute::eMarker: return "marker"; 
		case FbxNodeAttribute::eSkeleton: return "skeleton"; 
		case FbxNodeAttribute::eMesh: return "mesh"; 
		case FbxNodeAttribute::eNurbs: return "nurbs"; 
		case FbxNodeAttribute::ePatch: return "patch"; 
		case FbxNodeAttribute::eCamera: return "camera"; 
		case FbxNodeAttribute::eCameraStereo: return "stereo"; 
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher"; 
		case FbxNodeAttribute::eLight: return "light"; 
		case FbxNodeAttribute::eOpticalReference: return "optical reference"; 
		case FbxNodeAttribute::eOpticalMarker: return "marker"; 
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve"; 
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface"; 
		case FbxNodeAttribute::eBoundary: return "boundary"; 
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface"; 
		case FbxNodeAttribute::eShape: return "shape"; 
		case FbxNodeAttribute::eLODGroup: return "lodgroup"; 
		case FbxNodeAttribute::eSubDiv: return "subdiv"; 
		default: return "unknown"; 
	} 
}
/* Tab character ("\t") counter */
int numTabs = 0; 

/**
 * Print the required number of tabs.
 */
void PrintTabs() {
	for(int i = 0; i < numTabs; i++)
		std::cout<<"\t";
}

void PrintAttribute(FbxNodeAttribute* pAttribute) {
	if(!pAttribute) return;

	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
	std::cout << "<attribute type=" << typeName.Buffer() <<"name="<< attrName.Buffer()<<"/>"<< std::endl;
}

void constructMesh(FbxMesh* fbxMesh,Mesh* m, std::map<std::string, std::string>& textures)
{
	std::cout << "CONSTRUCT MESH ::" << m->name <<  std::endl;
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FbxArray< FbxVector4 > pNormals;
	fbxMesh->GetPolygonVertexNormals (pNormals);
	for(int i=0;i < pNormals.Size();i++){
		glm::vec3 n;
		n.x = pNormals [i][0];
		n.y = pNormals [i][1];
		n.z = pNormals [i][2];
		temp_normals.push_back(n);
	}

	FbxArray< FbxVector2 > pUVs;

	FbxStringList lUVSetNameList;
	fbxMesh->GetUVSetNames(lUVSetNameList);
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		std::cout << lUVSetNameList.GetStringAt(lUVSetIndex) << std::endl;
		fbxMesh->GetPolygonVertexUVs( lUVSetNameList.GetStringAt(lUVSetIndex),pUVs);
	}
	std::cout << "UVs size::" << pUVs.Size() << std::endl;
	for(int i=0;i < pUVs.Size();i++){
		glm::vec2 uv;
		uv.x = pUVs [i][0];
		uv.y = pUVs [i][1];
		temp_uvs.push_back(uv);
	}

	int cont=0;
	for(int i=0;i < fbxMesh->GetPolygonCount();i++){
		if(fbxMesh->GetPolygonSize(i)==3)
		{
			for(int j=0;j<3;j++){
				vertexIndices.push_back(fbxMesh->GetPolygonVertex(i,j));
				int v_index = fbxMesh->GetPolygonVertex(i,j);

				m->normals.push_back(temp_normals[cont]);

				if(temp_uvs.size())
					m->uvs.push_back(temp_uvs[cont]);
				cont ++;

			}
		}
		else if(fbxMesh->GetPolygonSize(i)==4)
		{
			int a = fbxMesh->GetPolygonVertex(i,0);
			int b = fbxMesh->GetPolygonVertex(i,1);
			int c = fbxMesh->GetPolygonVertex(i,2);
			int d = fbxMesh->GetPolygonVertex(i,3);

			vertexIndices.push_back(a);
			vertexIndices.push_back(b);
			vertexIndices.push_back(c);
			vertexIndices.push_back(a);
			vertexIndices.push_back(c);
			vertexIndices.push_back(d);

			m->normals.push_back(temp_normals[cont]);
			m->normals.push_back(temp_normals[cont+1]);
			m->normals.push_back(temp_normals[cont+2]);
			m->normals.push_back(temp_normals[cont]);
			m->normals.push_back(temp_normals[cont+2]);
			m->normals.push_back(temp_normals[cont+3]);


			if(temp_uvs.size()){

				m->uvs.push_back(temp_uvs[cont]);
				m->uvs.push_back(temp_uvs[cont+1]);
				m->uvs.push_back(temp_uvs[cont+2]);
				m->uvs.push_back(temp_uvs[cont]);
				m->uvs.push_back(temp_uvs[cont+2]);
				m->uvs.push_back(temp_uvs[cont+3]);

			}

			cont +=4;




		}else if(fbxMesh->GetPolygonSize(i)==5){

			int a = fbxMesh->GetPolygonVertex(i,0);
			int b = fbxMesh->GetPolygonVertex(i,1);
			int c = fbxMesh->GetPolygonVertex(i,2);
			int d = fbxMesh->GetPolygonVertex(i,3);
			int e = fbxMesh->GetPolygonVertex(i,4);

			vertexIndices.push_back(a);
			vertexIndices.push_back(b);
			vertexIndices.push_back(c);
			vertexIndices.push_back(a);
			vertexIndices.push_back(c);
			vertexIndices.push_back(d);
			vertexIndices.push_back(a);
			vertexIndices.push_back(d);
			vertexIndices.push_back(e);


			m->normals.push_back(temp_normals[cont]);
			m->normals.push_back(temp_normals[cont+1]);
			m->normals.push_back(temp_normals[cont+2]);
			m->normals.push_back(temp_normals[cont]);
			m->normals.push_back(temp_normals[cont+2]);
			m->normals.push_back(temp_normals[cont+3]);
			m->normals.push_back(temp_normals[cont]);
			m->normals.push_back(temp_normals[cont+3]);
			m->normals.push_back(temp_normals[cont+4]);


			if(temp_uvs.size()){
				m->uvs.push_back(temp_uvs[cont]);
				m->uvs.push_back(temp_uvs[cont+1]);
				m->uvs.push_back(temp_uvs[cont+2]);
				m->uvs.push_back(temp_uvs[cont]);
				m->uvs.push_back(temp_uvs[cont+2]);
				m->uvs.push_back(temp_uvs[cont+3]);
				m->uvs.push_back(temp_uvs[cont]);
				m->uvs.push_back(temp_uvs[cont+3]);
				m->uvs.push_back(temp_uvs[cont+4]);
			}
			cont +=5;
		}
	}


	FbxVector4* vs = fbxMesh->GetControlPoints();
	for(int i=0;i < fbxMesh->GetControlPointsCount();i++){
		glm::vec4 vertex;
		vertex.x = vs [i][0];
		vertex.y = vs [i][1];
		vertex.z = vs [i][2];
		vertex.w = 1;


		temp_vertices.push_back(glm::vec3(vertex.x,vertex.y,vertex.z));

	}




		// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex];
		//glm::vec2 uv = fbxMesh->[ uvIndex-1
		//glm::vec3 normal = temp_normals[ i];
		//m->normals.push_back(normal); 

		// Put the attributes in buffers
		m->vertices.push_back(vertex);
		//out_uvs     .push_back(uv);

	}
	std::map<std::string, std::string>::iterator it = textures.begin();
    while(it != textures.end())
    {
    	std::cout << "LOAD TEXTURE FOR "<< m->name <<" TYPE : " <<it->first<<std::endl;
        if(it->first=="DiffuseColor"){
        	m->loadTextureDiff(it->second,false);
        }else if(it->first=="NormalMap"){
        	m->loadTextureNormal(it->second,false);
        }else if(it->first=="SpecularColor"){
        	m->loadTextureSpec(it->second,false);
        }else if(it->first=="Bump"){
        	m->loadTextureBump(it->second,false);
        }else if(it->first=="EmissiveColor"){
        	m->loadTextureEmissive(it->second,false);
        }
        it++;
	}

	std::cout << "END CONSTRUCT MESH ::" << m->name <<  std::endl;
	m->loadMesh();

}



void PrintNode(FbxNode* pNode,glm::mat4 model,Mesh* m, Node* node,float scale) {

	const char* lSkeletonTypes[] = { "Root", "Limb", "Limb Node", "Effector" };
	PrintTabs();
	const char* nodeName = pNode->GetName();
	FbxDouble3 translation = pNode->LclTranslation.Get(); 
	FbxDouble3 rotation = pNode->LclRotation.Get(); 
	FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
	std::cout <<"<node name="<< nodeName <<" translation="<<translation[0]<<" " << translation[1]<<" " << translation[2]
	<< " rotation="<<  rotation[0]<<" " << rotation[1]<<" " << rotation[2] 
	<< " scaling=" <<scaling[0]<<" " << scaling[1]<<" " << scaling[2]<< std::endl; 

	glm::vec3 gtranslation;
	gtranslation.x=translation[0];
	gtranslation.y=translation[1];
	gtranslation.z=translation[2];

	glm::vec3 grotation;
	grotation.x=rotation[0];
	grotation.y=rotation[1];
	grotation.z=rotation[2];

	glm::vec3 gscaling;
	gscaling.x=scaling[0];
	gscaling.y=scaling[1];
	gscaling.z=scaling[2];
	model = glm::translate(model,gtranslation);	
	model = glm::rotate(model,glm::radians(grotation.x),glm::vec3(1.0f,0.0f,0.0f));
	model = glm::rotate(model,glm::radians(grotation.y),glm::vec3(0.0f,1.0f,0.0f));
	model = glm::rotate(model,glm::radians(grotation.z),glm::vec3(0.0f,0.0f,1.0f));
	model = glm::scale(model,gscaling);	
	numTabs++;

	if(pNode->GetGeometry())
		DisplayTexture(pNode->GetGeometry());

    // Print the node's attributes.
	for(int i = 0; i < pNode->GetNodeAttributeCount(); i++){
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));
		if( pNode->GetNodeAttributeByIndex(i)->GetAttributeType()==FbxNodeAttribute::eMesh){
			
			Mesh* msh = new Mesh(m->texturePath);
			msh->name= std::string(nodeName);
			m->vmesh.push_back(msh);


			/*glm::vec3 gtranslation;
			gtranslation.x=translation[0];
			gtranslation.y=translation[1];
			gtranslation.z=translation[2];

			glm::vec3 grotation;
			grotation.x=rotation[0];
			grotation.y=rotation[1];
			grotation.z=rotation[2];

			glm::vec3 gscaling;
			gscaling.x=scaling[0];
			gscaling.y=scaling[1];
			gscaling.z=scaling[2];*/

			msh->ModelMatrix = model; 
			/*msh->ModelMatrix glm::translate(msh->ModelMatrix,gtranslation);	
			msh->ModelMatrix = glm::rotate(msh->ModelMatrix,glm::radians(grotation.x),glm::vec3(1.0f,0.0f,0.0f));
			msh->ModelMatrix = glm::rotate(msh->ModelMatrix,glm::radians(grotation.y),glm::vec3(0.0f,1.0f,0.0f));
			msh->ModelMatrix = glm::rotate(msh->ModelMatrix,glm::radians(grotation.z),glm::vec3(0.0f,0.0f,1.0f));
			msh->ModelMatrix = glm::scale(msh->ModelMatrix,gscaling);*/


    		std::map<std::string, std::string> textures;


			getTextures(pNode->GetGeometry(),textures);
			constructMesh(pNode->GetMesh(),msh,textures);
			m = msh;


		}else if( pNode->GetNodeAttributeByIndex(i)->GetAttributeType()==FbxNodeAttribute::eSkeleton){
			FbxSkeleton* lSkeleton= pNode->GetSkeleton();
			
			FbxAMatrix fbxam = GetGlobalPosition(pNode,0,NULL,NULL);
			translation = fbxam.GetT ();
			rotation = fbxam.GetR ();
			scaling = fbxam.GetS ();

			glm::vec3 gtranslation;
			gtranslation.x=translation[0];
			gtranslation.y=translation[1];
			gtranslation.z=translation[2];



			glm::vec3 grotation;
			grotation.x=rotation[0];
			grotation.y=rotation[1];
			grotation.z=rotation[2];

			glm::vec3 gscaling;
			gscaling.x=scaling[0];
			gscaling.y=scaling[1];
			gscaling.z=scaling[2];


			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix,gtranslation/scale);	
			ModelMatrix = glm::rotate(ModelMatrix,glm::radians(grotation.x),glm::vec3(1.0f,0.0f,0.0f));
			ModelMatrix = glm::rotate(ModelMatrix,glm::radians(grotation.y),glm::vec3(0.0f,1.0f,0.0f));
			ModelMatrix = glm::rotate(ModelMatrix,glm::radians(grotation.z),glm::vec3(0.0f,0.0f,1.0f));
			ModelMatrix = glm::scale(ModelMatrix,gscaling);	

			Node* next;
			if(node->label=="init")
			{
				node->label = std::string(nodeName);
				node->ModelMatrix = glm::mat4(ModelMatrix);
				next=node;

			}else{
				next=new Node(node,std::string(nodeName));
				next->ModelMatrix = glm::mat4(ModelMatrix);
				node->addChild(next);
				node=next;
			}
			
			//FbxColor c = p->GetLimbNodeColor () ;
			//std::cout << "COLOR : " << c.mRed << " " << c.mGreen <<" "<< c.mBlue << endl;

			std::cout <<"Type: " << lSkeletonTypes[lSkeleton->GetSkeletonType()] << std::endl;
			if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimb)
			{
				std::cout << "    Limb Length: " << lSkeleton->LimbLength.Get() << std::endl;
			}
			else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
			{
				std::cout << "	Limb Node Size: " <<  lSkeleton->Size.Get() << std::endl;
			}
			else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eRoot)
			{
				std::cout << "    Limb Root Size: "<< lSkeleton->Size.Get() << std::endl;
			}
		}
	}

    // Recursively print the children.
	for(int j = 0; j < pNode->GetChildCount(); j++)
		PrintNode(pNode->GetChild(j),model,m,node,scale);

	numTabs--;
	PrintTabs();
	std::cout<<"</node>"<<std::endl;
}


bool loadFBX(
	const char * path, 
	Object3D* obj,
	Skeleton* skel,
	float scale
	){

      // Change the following filename to a suitable filename value.
    // Initialize the SDK manager. This object handles memory management.
	FbxManager* lSdkManager = FbxManager::Create();

	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

    // Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");

    // Use the first argument as the filename for the importer.
	bool lImportStatus = lImporter->Initialize(path, -1, lSdkManager->GetIOSettings());
	if(!lImportStatus) {
		printf("Call to FbxImporter::Initialize() failed.\n"); 
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString()); 
		exit(-1);
	}


	FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene");

    // Import the contents of the file into the scene.
	lImporter->Import(lScene);

    // The file is imported, so get rid of the importer.
	lImporter->Destroy();

	FbxNode* lRootNode = lScene->GetRootNode();
	glm::mat4 modelMatCumul=glm::mat4(1.0);
	Node* root = skel->root;

	if(lRootNode) {
		for(int i = 0; i < lRootNode->GetChildCount(); i++)
			PrintNode(lRootNode->GetChild(i),modelMatCumul,obj->rootMesh,root,scale);
	}
	
    // Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();
	return true;
}

#ifdef USE_ASSIMP // don't use this #define, it's only for me (it AssImp fails to compile on your machine, at least all the other tutorials still work)

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
	){

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
	if( !scene) {
		fprintf( stderr, importer.GetErrorString());
		getchar();
		return false;
	}
	const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

	// Fill vertices positions
	vertices.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
	}

	// Fill vertices texture coordinates
	uvs.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
		uvs.push_back(glm::vec2(UVW.x, UVW.y));
	}

	// Fill vertices normals
	normals.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D n = mesh->mNormals[i];
		normals.push_back(glm::vec3(n.x, n.y, n.z));
	}


	// Fill face indices
	indices.reserve(3*mesh->mNumFaces);
	for (unsigned int i=0; i<mesh->mNumFaces; i++){
		// Assume the model has only triangles.
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}
	
	// The "scene" pointer will be deleted automatically by "importer"
	return true;
}

#endif