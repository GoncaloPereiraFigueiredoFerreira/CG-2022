
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
    bool flag = true;
        if (fds && flag){

        doc.parse<0>(&fds[0]);
        xml_node<> * root;
        xml_node<> * t1;
        xml_node<> * t2;
        xml_node<> * t3;


        
        root = doc.first_node("world");
        if (root==0)throw new exception();
        

        t1 = root->first_node("camera");
        if (t1==0) throw new exception();
        
        t2 = t1->first_node("position");
        if (t2==0) throw new exception();
        



        xml.cameraInfo.xPos = atoi(t2->first_attribute("x")->value());
        xml.cameraInfo.yPos = atoi(t2->first_attribute("y")->value());
        xml.cameraInfo.zPos = atoi(t2->first_attribute("z")->value());


        t2 = t2->next_sibling();
        if (t2==0) throw new exception();
        xml.cameraInfo.xLook = atoi(t2->first_attribute("x")->value());
        xml.cameraInfo.yLook = atoi(t2->first_attribute("y")->value());
        xml.cameraInfo.zLook = atoi(t2->first_attribute("z")->value());

        t2 = t2->next_sibling();
        if (t2==0) throw new exception();
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
            xml.cameraInfo.near = atoi(t2->first_attribute("near")->value());
            xml.cameraInfo.far = atoi(t2->first_attribute("far")->value());
        }

        t1 = root->first_node("lights");
        if (t1==0) throw new exception();
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
                ls.dirX =atoi(t2->first_attribute("dirX")->value());
                ls.dirY =atoi(t2->first_attribute("dirY")->value());
                ls.dirZ =atoi(t2->first_attribute("dirZ")->value());
                ls.cutoff = atoi(t2->first_attribute("cutoff")->value());
                xml.lightsList.spotL.push_back(ls);
            }
        }
        t1 = root->first_node("group");
        if (t1==0) throw new exception();
        t2 = t1->first_node("transform");
        if (t2==0) throw new exception();
        i=0;
    
        for (t2 = t2->first_node(); t2; i++,t2 = t2->next_sibling()){
            string s(t2->name());
            if (s == "translate"){
                Translate t;
                t.order=i;
                t.x = atoi(t2->first_attribute("X")->value());
                t.y = atoi(t2->first_attribute("Y")->value());
                t.z = atoi(t2->first_attribute("Z")->value());
                xml.transforms.transl =t;
            }
            else if ( s == "rotate"){
                Rotate r;
                r.order=i;
                r.angle= atoi(t2->first_attribute("angle")->value());
                r.x =atoi(t2->first_attribute("axisX")->value());
                r.y =atoi(t2->first_attribute("axisY")->value());
                r.z =atoi(t2->first_attribute("axisZ")->value());
                xml.transforms.rotate =r;
            }
            else if ( s== "scale"){
                Scale s;
                s.order=i;
                s.x =atoi(t2->first_attribute("X")->value());
                s.y =atoi(t2->first_attribute("Y")->value());
                s.z =atoi(t2->first_attribute("Z")->value());
                xml.transforms.scale =s;
            }
        }


        t1 = t1->first_node("models");
        if (t1==0) throw new exception();
        t2 = t1->first_node("model");
        if (t2==0) throw new exception();
        
        for ( ; t2; t2 = t2->next_sibling()){
            Color defaultC;
            defaultC.diffuseR=200;
            defaultC.diffuseG=200;
            defaultC.diffuseB=200;
            defaultC.specularR=0;
            defaultC.specularG=0;
            defaultC.specularB=0;
            defaultC.emissiveR=0;
            defaultC.emissiveG=0;
            defaultC.emissiveB=0;
            defaultC.ambientR=50;
            defaultC.ambientG=50;
            defaultC.ambientB=50;
            defaultC.shine=0;
            
            Model m;
            m.color=defaultC;
            m.sourceF = t2->first_attribute("file")->value();
            m.textureF =  NULL;
            m.color = defaultC;
            
            t3 = t2->first_node();
            if (t3!=0) {


            string name(t3->name());
            if (name == "texture"){
                m.textureF = t3->first_attribute()->value();
                t3= t3->next_sibling();
                if (t3) name.assign(t3->name());
            }
            if (name == "color"){
                t3 = t3->first_node("diffuse");

                m.color.diffuseR = atoi(t3->first_attribute("R")->value());
                m.color.diffuseG = atoi(t3->first_attribute("G")->value());
                m.color.diffuseB = atoi(t3->first_attribute("B")->value());

                t3 = t3->next_sibling("ambient");

                m.color.ambientR = atoi(t3->first_attribute("R")->value()); 



                m.color.ambientG = atoi(t3->first_attribute("G")->value());
                m.color.ambientB = atoi(t3->first_attribute("B")->value());

                t3 = t3->next_sibling("specular");

                m.color.specularR = atoi(t3->first_attribute("R")->value());
                m.color.specularG = atoi(t3->first_attribute("G")->value());
                m.color.specularB = atoi(t3->first_attribute("B")->value());

                t3 = t3->next_sibling("emissive");

                m.color.emissiveR = atoi(t3->first_attribute("R")->value());
                m.color.emissiveG = atoi(t3->first_attribute("G")->value());
                m.color.emissiveB = atoi(t3->first_attribute("B")->value());

                t3 = t3->next_sibling("shininess");
                m.color.shine =  atoi(t3->first_attribute("value")->value());

            }
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