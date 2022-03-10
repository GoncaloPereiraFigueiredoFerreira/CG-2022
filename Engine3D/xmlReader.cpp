
//Module to read the information inside the XML files
#include "xmlReader.hpp"
#include <string>

using namespace std;
using namespace rapidxml;

char* readFile(string filename){
	fstream my_file;
    string text;
    string wholeF;
	my_file.open(filename, ios::in);
	if (!my_file) {
		cout << "No such file";
        return NULL;
	}
	else {
		while (getline(my_file,text)) {
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



 
Group captureGroups (xml_node<char> * root){
    Group main;
    xml_node<char> * t1;
    xml_node<char> * t2;

    t1 = root->first_node("transform");
    
    main.transforms.rotate.order=-1;
    main.transforms.transl.order=-1;
    main.transforms.scale.order =-1;
    if (t1!=0) {
        int i=0;
        // Capture Transformations
        for (t1 = t1->first_node(); t1; i++,t1 = t1->next_sibling()){
            string s(t1->name());
            if (s == "translate"){
                Translate t;
                t.order=i;
                t.x = atoi(t1->first_attribute("X")->value());
                t.y = atoi(t1->first_attribute("Y")->value());
                t.z = atoi(t1->first_attribute("Z")->value());
                main.transforms.transl =t;
            }
            else if ( s == "rotate"){
                Rotate r;
                r.order=i;
                r.angle= atoi(t1->first_attribute("angle")->value());
                r.x =atoi(t1->first_attribute("axisX")->value());
                r.y =atoi(t1->first_attribute("axisY")->value());
                r.z =atoi(t1->first_attribute("axisZ")->value());
                main.transforms.rotate =r;
            }
            else if ( s== "scale"){
                Scale s;
                s.order=i;
                s.x =atoi(t1->first_attribute("X")->value());
                s.y =atoi(t1->first_attribute("Y")->value());
                s.z =atoi(t1->first_attribute("Z")->value());            
                main.transforms.scale =s;
            }   
        }
    }

    // Capture Models (optional)
    t1 = root->first_node("models");
        
    if (t1!=0) {
        t1 = t1->first_node("model");
            
        if (t1==0) throw new exception;
            

        for ( ; t1; t1 = t1->next_sibling()){
            Model m;
            m.sourceF = t1->first_attribute("file")->value();
            m.textureF =  NULL;
             
             //Setting the default values
            m.color.diffuseR=200;
            m.color.diffuseG=200;
            m.color.diffuseB=200;
            m.color.specularR=0;
            m.color.specularG=0;
            m.color.specularB=0;
            m.color.emissiveR=0;
            m.color.emissiveG=0;
            m.color.emissiveB=0;
            m.color.ambientR=50;
            m.color.ambientG=50;
            m.color.ambientB=50;
            m.color.shine=0;
            t2 = t1->first_node();
            if (t2!=0) {
            string name(t2->name());
            if (name == "texture"){
                m.textureF = t2->first_attribute()->value();
                t2= t2->next_sibling();
                if (t2) name.assign(t2->name());
            }
            if (name == "color"){
                t2 = t2->first_node("diffuse");
                m.color.diffuseR = atoi(t2->first_attribute("R")->value());
                m.color.diffuseG = atoi(t2->first_attribute("G")->value());
                m.color.diffuseB = atoi(t2->first_attribute("B")->value());
                t2 = t2->next_sibling("ambient");
                m.color.ambientR = atoi(t2->first_attribute("R")->value()); 
                m.color.ambientG = atoi(t2->first_attribute("G")->value());
                m.color.ambientB = atoi(t2->first_attribute("B")->value());
                t2 = t2->next_sibling("specular");
                m.color.specularR = atoi(t2->first_attribute("R")->value());
                m.color.specularG = atoi(t2->first_attribute("G")->value());
                m.color.specularB = atoi(t2->first_attribute("B")->value());
                t2 = t2->next_sibling("emissive");
                m.color.emissiveR = atoi(t2->first_attribute("R")->value());
                m.color.emissiveG = atoi(t2->first_attribute("G")->value());
                m.color.emissiveB = atoi(t2->first_attribute("B")->value());
                t2 = t2->next_sibling("shininess");
                m.color.shine =  atoi(t2->first_attribute("value")->value());
            }
            }
            main.modelList.push_back(m);
        }
    }
    // Capture other nested groups
    for (t1 = root->first_node("group"); t1; t1= t1->next_sibling()){
        string name(t1->name());
        if (name == "group"){
            Group temp = captureGroups(t1);
            main.groupChildren.push_back(temp);
            
        }
    }

    return main;    
}



xmlInfo readXML(string filename){
    xml_document<> doc;
    
    char *fds = readFile(filename);
   
    xmlInfo xml;        
    bool flag = true;
        if (fds && flag){

        doc.parse<0>(&fds[0]);
        xml_node<> * root;
        xml_node<> * t1;
        xml_node<> * t2;
        xml_node<> * t3;


        // Mandatory
        root = doc.first_node("world");
        if (root==0)throw new exception();
        
        // Mandatory
        t1 = root->first_node("camera");
        if (t1==0) throw new exception();
        
        // Mandatory
        t2 = t1->first_node("position");
        if (t2==0) throw new exception();
        

        //Capture X Y and Z Camera Positions

        xml.cameraInfo.xPos = atoi(t2->first_attribute("x")->value());
        xml.cameraInfo.yPos = atoi(t2->first_attribute("y")->value());
        xml.cameraInfo.zPos = atoi(t2->first_attribute("z")->value());

        //Capture X Y and Z LookAt Positions

        t2 = t2->next_sibling();
        if (t2==0) throw new exception();
        xml.cameraInfo.xLook = atoi(t2->first_attribute("x")->value());
        xml.cameraInfo.yLook = atoi(t2->first_attribute("y")->value());
        xml.cameraInfo.zLook = atoi(t2->first_attribute("z")->value());

        //Default Values for Up and Projection
        xml.cameraInfo.xUp = 0;
        xml.cameraInfo.yUp = 1;
        xml.cameraInfo.zUp = 0;
        xml.cameraInfo.fov = 60;
        xml.cameraInfo.near = 1;
        xml.cameraInfo.far = 1000;

        //Capture optional Up and Projection
        t2 = t2->next_sibling();
        if (t2!=0) {
            string t(t2->name());
    
            if (t== "up"){
                xml.cameraInfo.xUp = atoi(t2->first_attribute("x")->value());
                xml.cameraInfo.yUp = atoi(t2->first_attribute("y")->value());
                xml.cameraInfo.zUp = atoi(t2->first_attribute("z")->value());
                t2 = t2->next_sibling();
                if (t2) t.assign(t2->name());
            }
            if (t== "projection"){
                xml.cameraInfo.fov = atoi(t2->first_attribute("fov")->value());
                xml.cameraInfo.near= atoi(t2->first_attribute("near")->value());
                xml.cameraInfo.far = atoi(t2->first_attribute("far")->value());
            }
        }


        // Capture Lights //optional
        t1 = root->first_node("lights");
        if (t1!=0) {
            t2 = t1->first_node("light");
            int i=0;
            for (; i<8 && t2; i++,t2= t2->next_sibling()){
                string s( t2->first_attribute("type")->value());
                if (s == "point"){
                    LPoint lp;
                    lp.posX = atoi(t2->first_attribute("posX")->value());
                    lp.posY = atoi(t2->first_attribute("posY")->value());
                    lp.posZ = atoi(t2->first_attribute("posZ")->value());
                    xml.lightsList.points.push_back(lp);
                }
                else if (s== "directional"){
                    LDirec ld;
                    ld.dirX =atoi(t2->first_attribute("dirX")->value());
                    ld.dirY =atoi(t2->first_attribute("dirY")->value());
                    ld.dirZ =atoi(t2->first_attribute("dirZ")->value());
                    xml.lightsList.direct.push_back(ld);
                }
                else if (s=="spotlight"){
                    LSpotl ls;
                    ls.posX = atoi(t2->first_attribute("posX")->value());
                    ls.posY = atoi(t2->first_attribute("posY")->value());
                    ls.posZ = atoi(t2->first_attribute("posZ")->value());
                    ls.dirX = atoi(t2->first_attribute("dirX")->value());
                    ls.dirY = atoi(t2->first_attribute("dirY")->value());
                    ls.dirZ = atoi(t2->first_attribute("dirZ")->value());
                    ls.cutoff = atoi(t2->first_attribute("cutoff")->value());
                    xml.lightsList.spotL.push_back(ls);
                }
            }
        }
        //Capture groups
        xml.groups = captureGroups(root->first_node("group"));



    
/*

 
        cout <<"Far:" <<xml.cameraInfo.far << "\n";
        cout <<"xPos:" <<xml.cameraInfo.xPos << "\n";
        cout <<"yPos:" <<xml.cameraInfo.yPos << "\n";
        cout <<"zPos:" <<xml.cameraInfo.zPos << "\n";
        cout <<"Fov:" <<xml.cameraInfo.fov << "\n\n";

        cout <<"DirX:" <<xml.lightsList.direct[0].dirX << "\n\n";

        cout <<"Cut:" <<xml.lightsList.spotL[0].cutoff << "\n\n";

        cout <<"pointsX:" <<xml.lightsList.points[0].posX << "\n\n";

        cout <<"angle:" <<xml.groups.transforms.rotate.angle << "\n\n";

        cout <<"angle:" <<xml.groups.transforms.transl.order << "\n\n";

        cout <<"angle:" <<xml.groups.transforms.scale.order << "\n\n";


        cout <<"SHEEEEEESh: " <<xml.groups.groupChildren[0].transforms.scale.order << "\n\n";
        cout <<"SHEEEEEESh: " <<xml.groups.groupChildren[0].transforms.scale.x << "\n\n";

        cout  <<xml.groups.modelList[0].sourceF << "\n\n";

        cout  <<xml.groups.modelList[0].color.shine << "\n\n";

        cout  <<xml.groups.modelList[0].color.ambientB << "\n\n";
*/
        }
        return xml;

    }



    

    
    int main(int argc, char **argv){
        

       xmlInfo x = readXML("xml_syntax.xml");
      // cout<< x.modelList[0].sourceF << "\n";
    }
    