
//Module to read the information inside the XML files
#include "xmlReader.hpp"

using namespace std;
using namespace rapidxml;

char* readFile(char * filename){
	fstream my_file;
    string text;
    string wholeF;
	my_file.open(filename, ios::in);
	if (!my_file) {
		cout << "No such file";
        return NULL;
	}
	else {
		while (getline (my_file,text)) {
            wholeF.append(text);
            wholeF.append("\n");
		}

	}
	my_file.close();
    
    char *fds = (char*) malloc((wholeF.size()+1) * sizeof(char)) ;
    wholeF.copy(fds,wholeF.size(),0);
    fds[wholeF.size()] = '\0';

    return fds;
}


xmlInfo readXML(){
    xml_document<> doc;
    char *fds = readFile((char*) "xml_syntax.xml");
    xmlInfo xml;
    if (fds){

    doc.parse<0>(&fds[0]);
    xml_node<> * root_node;
    xml_node<> * temp1_node;
    xml_node<> * temp2_node;
    xml_node<> * temp3_node;


    //faltam fazer verificações
    root_node = doc.first_node("world");
    

    temp1_node = root_node->first_node("camera");
    temp2_node = temp1_node->first_node("position");

    xml.cameraInfo.xPos = atoi(temp2_node->first_attribute("x")->value());
    xml.cameraInfo.yPos = atoi(temp2_node->first_attribute("y")->value());
    xml.cameraInfo.zPos = atoi(temp2_node->first_attribute("z")->value());
    

    temp2_node = temp2_node->next_sibling();
    xml.cameraInfo.xLook = atoi(temp2_node->first_attribute("x")->value());
    xml.cameraInfo.yLook = atoi(temp2_node->first_attribute("y")->value());
    xml.cameraInfo.zLook = atoi(temp2_node->first_attribute("z")->value());

    temp2_node = temp2_node->next_sibling();
    string t(temp2_node->name());
    if (t== "up"){
        xml.cameraInfo.xUp = atoi(temp2_node->first_attribute("x")->value());
        xml.cameraInfo.yUp = atoi(temp2_node->first_attribute("y")->value());
        xml.cameraInfo.zUp = atoi(temp2_node->first_attribute("z")->value());
        temp2_node = temp2_node->next_sibling();
        if (temp2_node) t.assign(temp2_node->name());
    }
    if (t== "projection"){
        xml.cameraInfo.fov = atoi(temp2_node->first_attribute("fov")->value());
        xml.cameraInfo.near = atoi(temp2_node->first_attribute("near")->value());
        xml.cameraInfo.far = atoi(temp2_node->first_attribute("far")->value());
    }

    temp1_node = root_node->first_node("lights");
    
    int i=0;
    for (temp2_node = temp1_node->first_node("light"); i<8 && temp2_node; i++,temp2_node= temp2_node->next_sibling()){
        string s( temp2_node->first_attribute("type")->value());
        if (s == "point"){
            LPoint lp;
            lp.posX = atoi(temp2_node->first_attribute("posX")->value());
            lp.posY = atoi(temp2_node->first_attribute("posY")->value());
            lp.posZ = atoi(temp2_node->first_attribute("posZ")->value());
            xml.lightsList.points.push_back(lp);
        }
        else if (s== "directional"){
            LDirec ld;
            ld.dirX =atoi(temp2_node->first_attribute("dirX")->value());
            ld.dirY =atoi(temp2_node->first_attribute("dirY")->value());
            ld.dirZ =atoi(temp2_node->first_attribute("dirZ")->value());
            xml.lightsList.direct.push_back(ld);
        }
        else if (s=="spotlight"){
            LSpotl ls;
            ls.posX = atoi(temp2_node->first_attribute("posX")->value());
            ls.posY = atoi(temp2_node->first_attribute("posY")->value());
            ls.posZ = atoi(temp2_node->first_attribute("posZ")->value());
            ls.dirX =atoi(temp2_node->first_attribute("dirX")->value());
            ls.dirY =atoi(temp2_node->first_attribute("dirY")->value());
            ls.dirZ =atoi(temp2_node->first_attribute("dirZ")->value());
            ls.cutoff = atoi(temp2_node->first_attribute("cutoff")->value());
            xml.lightsList.spotL.push_back(ls);
        }
    }
    temp1_node = root_node->first_node("group");
    temp2_node = temp1_node->first_node("transform");
    i=0;
 
    for (temp2_node = temp2_node->first_node(); temp2_node; i++,temp2_node = temp2_node->next_sibling()){
        string s(temp2_node->name());
        if (s == "translate"){
            Translate t;
            t.order=i;
            t.x = atoi(temp2_node->first_attribute("X")->value());
            t.y = atoi(temp2_node->first_attribute("Y")->value());
            t.z = atoi(temp2_node->first_attribute("Z")->value());
            xml.transforms.transl =t;
        }
        else if ( s == "rotate"){
            Rotate r;
            r.order=i;
            r.angle= atoi(temp2_node->first_attribute("angle")->value());
            r.x =atoi(temp2_node->first_attribute("axisX")->value());
            r.y =atoi(temp2_node->first_attribute("axisY")->value());
            r.z =atoi(temp2_node->first_attribute("axisZ")->value());
            xml.transforms.rotate =r;
        }
        else if ( s== "scale"){
            Scale s;
            s.order=i;
            s.x =atoi(temp2_node->first_attribute("X")->value());
            s.y =atoi(temp2_node->first_attribute("Y")->value());
            s.z =atoi(temp2_node->first_attribute("Z")->value());
            xml.transforms.scale =s;
        }
    }
    

    temp1_node = temp1_node->first_node("models");
     
    for ( temp2_node = temp1_node->first_node("model"); temp2_node; temp2_node = temp2_node->next_sibling()){
        Model m;
        m.sourceF = temp2_node->first_attribute("file")->value();
        m.textureF =  NULL;
        
        temp3_node = temp2_node->first_node();
        
    
        string name(temp3_node->name());
        if (name == "texture"){
            m.textureF = temp3_node->first_attribute()->value();
            temp3_node= temp3_node->next_sibling();
            if (temp3_node) name.assign(temp3_node->name());
        }
        if (name == "color"){
            temp3_node = temp3_node->first_node("diffuse");
            
            m.color.diffuseR = atoi(temp3_node->first_attribute("R")->value());
            m.color.diffuseG = atoi(temp3_node->first_attribute("G")->value());
            m.color.diffuseB = atoi(temp3_node->first_attribute("B")->value());

            temp3_node = temp3_node->next_sibling("ambient");
           
            m.color.ambientR = atoi(temp3_node->first_attribute("R")->value()); 
    
            
            
            m.color.ambientG = atoi(temp3_node->first_attribute("G")->value());
            m.color.ambientB = atoi(temp3_node->first_attribute("B")->value());

            temp3_node = temp3_node->next_sibling("specular");

            m.color.specularR = atoi(temp3_node->first_attribute("R")->value());
            m.color.specularG = atoi(temp3_node->first_attribute("G")->value());
            m.color.specularB = atoi(temp3_node->first_attribute("B")->value());

            temp3_node = temp3_node->next_sibling("emissive");

            m.color.emissiveR = atoi(temp3_node->first_attribute("R")->value());
            m.color.emissiveG = atoi(temp3_node->first_attribute("G")->value());
            m.color.emissiveB = atoi(temp3_node->first_attribute("B")->value());

            temp3_node = temp3_node->next_sibling("shininess");
            m.color.shine =  atoi(temp3_node->first_attribute("value")->value());
            
        }
       
    
        xml.modelList.push_back(m);
    }

  
    

    /*
    cout <<"Far:" <<xml.cameraInfo.far << "\n";
    cout <<"xPos:" <<xml.cameraInfo.xPos << "\n";
    cout <<"yPos:" <<xml.cameraInfo.yPos << "\n";
    cout <<"zPos:" <<xml.cameraInfo.zPos << "\n";
    cout <<"Fov:" <<xml.cameraInfo.fov << "\n\n";
    
    cout <<"DirX:" <<xml.lightsList.direct[0].dirX << "\n\n";
    
    cout <<"Cut:" <<xml.lightsList.spotL[0].cutoff << "\n\n";

    cout <<"pointsX:" <<xml.lightsList.points[0].posX << "\n\n";

    cout <<"angle:" <<xml.transforms.rotate.angle << "\n\n";

    cout <<"angle:" <<xml.transforms.transl.order << "\n\n";

    cout <<"angle:" <<xml.transforms.scale.order << "\n\n";




    cout  <<xml.modelList[0].sourceF << "\n\n";

    cout  <<xml.modelList[0].color.shine << "\n\n";

    cout  <<xml.modelList[0].color.ambientB << "\n\n";

*/
    }
    return xml;

}

int main(int argc, char **argv){
   xmlInfo x = readXML();
   cout<< x.modelList[0].sourceF << "\n";
}