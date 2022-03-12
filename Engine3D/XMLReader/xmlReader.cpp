
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
        return nullptr;
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

    if (t1 != nullptr) {
        // Capture Transformations
        for (t1 = t1->first_node(); t1; t1 = t1->next_sibling()){
            string s(t1->name());
            if (s == "translate"){
                auto translate = new Translate(std::stod(t1->first_attribute("X")->value()),
                                       std::stod(t1->first_attribute("Y")->value()),
                                       std::stod(t1->first_attribute("Z")->value()));
                main.transforms.push_back(translate);
            }
            else if ( s == "rotate"){
                auto rotate = new Rotate(std::stod(t1->first_attribute("axisX")->value()),
                                    std::stod(t1->first_attribute("axisY")->value()),
                                    std::stod(t1->first_attribute("axisZ")->value()),
                                    std::stod(t1->first_attribute("angle")->value()));
                main.transforms.push_back(rotate);
            }
            else if ( s == "scale"){
                auto scale = new Scale(std::stod(t1->first_attribute("X")->value()),
                                   std::stod(t1->first_attribute("Y")->value()),
                                   std::stod(t1->first_attribute("Z")->value()));
                main.transforms.push_back(scale);
            }   
        }
    }

    // Capture Models (optional)
    t1 = root->first_node("models");
        
    if (t1 != nullptr) {
        t1 = t1->first_node("model");
            
        if (t1 == nullptr) throw new exception();
            

        for ( ; t1; t1 = t1->next_sibling()){
            ModelInfo m;
            m.sourceF = t1->first_attribute("file")->value();
            m.textureF = nullptr;
             
             //Setting the default values for color
             m.color = new Color(200, 200, 200,
                                 0, 0, 0,
                                 0, 0, 0,
                                 50, 50, 50,
                                 0);


            t2 = t1->first_node();
            if (t2 != nullptr) {
                string name(t2->name());
                if (name == "texture"){
                    m.textureF = t2->first_attribute()->value();
                    t2= t2->next_sibling();
                    if (t2) name.assign(t2->name());
                }
                if (name == "color"){
                    t2 = t2->first_node("diffuse");
                    m.color->diffuseR = std::stod(t2->first_attribute("R")->value());
                    m.color->diffuseG = std::stod(t2->first_attribute("G")->value());
                    m.color->diffuseB = std::stod(t2->first_attribute("B")->value());
                    t2 = t2->next_sibling("ambient");
                    m.color->ambientR = std::stod(t2->first_attribute("R")->value());
                    m.color->ambientG = std::stod(t2->first_attribute("G")->value());
                    m.color->ambientB = std::stod(t2->first_attribute("B")->value());
                    t2 = t2->next_sibling("specular");
                    m.color->specularR = std::stod(t2->first_attribute("R")->value());
                    m.color->specularG = std::stod(t2->first_attribute("G")->value());
                    m.color->specularB = std::stod(t2->first_attribute("B")->value());
                    t2 = t2->next_sibling("emissive");
                    m.color->emissiveR = std::stod(t2->first_attribute("R")->value());
                    m.color->emissiveG = std::stod(t2->first_attribute("G")->value());
                    m.color->emissiveB = std::stod(t2->first_attribute("B")->value());
                    t2 = t2->next_sibling("shininess");
                    m.color->shine =  std::stod(t2->first_attribute("value")->value());
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
        if (fds && flag) {

            doc.parse<0>(&fds[0]);
            xml_node<> *root;
            xml_node<> *t1;
            xml_node<> *t2;
            xml_node<> *t3;


            // Mandatory
            root = doc.first_node("world");
            if (root == nullptr)throw new exception();

            // Mandatory
            t1 = root->first_node("camera");
            if (t1 == nullptr) throw new exception();

            // Mandatory
            t2 = t1->first_node("position");
            if (t2 == nullptr) throw new exception();


            //Capture X Y and Z Camera Positions

            xml.cameraInfo.xPos = std::stod(t2->first_attribute("x")->value());
            xml.cameraInfo.yPos = std::stod(t2->first_attribute("y")->value());
            xml.cameraInfo.zPos = std::stod(t2->first_attribute("z")->value());

            //Capture X Y and Z LookAt Positions

            t2 = t2->next_sibling();
            if (t2 == nullptr) throw new exception();
            xml.cameraInfo.xLook = std::stod(t2->first_attribute("x")->value());
            xml.cameraInfo.yLook = std::stod(t2->first_attribute("y")->value());
            xml.cameraInfo.zLook = std::stod(t2->first_attribute("z")->value());

            //Default Values for Up and Projection
            xml.cameraInfo.xUp = 0;
            xml.cameraInfo.yUp = 1;
            xml.cameraInfo.zUp = 0;
            xml.cameraInfo.fov = 60;
            xml.cameraInfo.near = 1;
            xml.cameraInfo.far = 1000;

            //Capture optional Up and Projection
            t2 = t2->next_sibling();
            if (t2 != nullptr) {
                string t(t2->name());

                if (t == "up") {
                    xml.cameraInfo.xUp = std::stod(t2->first_attribute("x")->value());
                    xml.cameraInfo.yUp = std::stod(t2->first_attribute("y")->value());
                    xml.cameraInfo.zUp = std::stod(t2->first_attribute("z")->value());
                    t2 = t2->next_sibling();
                    if (t2) t.assign(t2->name());
                }
                if (t == "projection") {
                    xml.cameraInfo.fov = std::stod(t2->first_attribute("fov")->value());
                    xml.cameraInfo.near = std::stod(t2->first_attribute("near")->value());
                    xml.cameraInfo.far = std::stod(t2->first_attribute("far")->value());
                }
            }


            // Capture Lights //optional
            t1 = root->first_node("lights");
            if (t1 != nullptr) {
                t2 = t1->first_node("light");
                int i = 0;
                for (; i < 8 && t2; i++, t2 = t2->next_sibling()) {
                    string s(t2->first_attribute("type")->value());
                    if (s == "point") {
                        LPoint lp = LPoint(std::stod(t2->first_attribute("posX")->value()),
                                           std::stod(t2->first_attribute("posY")->value()),
                                           std::stod(t2->first_attribute("posZ")->value()));
                        xml.lightsList.points.push_back(lp);
                    } else if (s == "directional") {
                        LDirec ld = LDirec(std::stod(t2->first_attribute("dirX")->value()),
                                           std::stod(t2->first_attribute("dirY")->value()),
                                           std::stod(t2->first_attribute("dirZ")->value()));
                        xml.lightsList.direct.push_back(ld);
                    } else if (s == "spotlight") {
                        LSpotl ls = LSpotl(std::stod(t2->first_attribute("posX")->value()),
                                           std::stod(t2->first_attribute("posY")->value()),
                                           std::stod(t2->first_attribute("posZ")->value()),
                                           std::stod(t2->first_attribute("dirX")->value()),
                                           std::stod(t2->first_attribute("dirY")->value()),
                                           std::stod(t2->first_attribute("dirZ")->value()),
                                           ls.cutoff = std::stod(t2->first_attribute("cutoff")->value()));
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





    /*
    int main(int argc, char **argv){
        

       xmlInfo x = readXML("test_1_5.xml");
       cout << "XPOS " <<x.cameraInfo.xPos << "\n";
       cout << "yPOS " <<x.cameraInfo.yPos << "\n";
       cout << "zPOS " <<x.cameraInfo.zPos << "\n";
       cout << "lookX " <<x.cameraInfo.xLook << "\n";
       cout << "looky " <<x.cameraInfo.yLook << "\n";
       cout << "lookz " <<x.cameraInfo.zLook << "\n";
       cout << "upX " <<x.cameraInfo.xUp << "\n";
       cout << "upY " <<x.cameraInfo.yUp << "\n";
       cout << "upZ " <<x.cameraInfo.zUp << "\n";
       cout << "fov " <<x.cameraInfo.fov << "\n";
       cout << "near " <<x.cameraInfo.near << "\n";
       cout << "far " <<x.cameraInfo.far << "\n";
       cout << "Model "<< x.groups.modelList[0].sourceF << "\n";
       if (x.groups.modelList.size() == 2)  cout << "Model "<< x.groups.modelList[1].sourceF << "\n";
      // cout<< x.modelList[0].sourceF << "\n";
    }
    */
