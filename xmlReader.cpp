
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
    
    char fds[wholeF.size()+1];
    wholeF.copy(fds,wholeF.size(),0);
    fds[wholeF.size()] = '\0';

    return fds;
}


xmlInfo readXML(){
    xml_document<> doc;
    char *fds = readFile("xml_syntax.xml");
    xmlInfo xml;
    if (fds){

    doc.parse<0>(&fds[0]);
    xml_node<> * root_node;
    xml_node<> * temp1_node;
    xml_node<> * temp2_node;

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
    if (temp2_node->name() == "up"){
        xml.cameraInfo.xUp = atoi(temp2_node->first_attribute("x")->value());
        xml.cameraInfo.yUp = atoi(temp2_node->first_attribute("y")->value());
        xml.cameraInfo.zUp = atoi(temp2_node->first_attribute("z")->value());
        temp2_node = temp2_node->next_sibling();
    }
    if (temp2_node->name() == "projection"){
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
            Lights *l = &lp;
            xml.lightsList[i] =  lp;
            // well this fucks me up
            // how can we do Lights = LPoint or Lights = LSpotl
        }



    }






    }
    return xml;

}

int main(int argc, char **argv){
   readXML();
}