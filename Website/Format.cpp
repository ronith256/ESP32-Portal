#include <iostream>
#include <string>
using namespace std;

int main(){
    string str = "Piku<!>Goa Beach<@>false<#>false<$>false<%>Fcff<&>11.2587531|75.78041";
    string name = str.substr(0,str.find("<"));
    int pos1 = str.find("<!>");
    int pos2 = str.find("<@>");
    int pos3 = str.find("<#>");
    int pos4 = str.find("<$>");
    int pos5 = str.find("<%>");
    int pos6 = str.find("<&>");

    string location = str.substr(pos1+3,pos2-pos1 -3);
    string medical = str.substr(pos2+3,pos3-pos2-3);
    string fire = str.substr(pos3+3, pos4-pos3 -3);
    string collapse = str.substr(pos4+3, pos5-pos4-3);
    string message = str.substr(pos5+3, pos6-pos5 -3);
    string gps = str.substr(pos6+3);

    // cout << pos1 << endl;
    // cout << pos2 << endl;
    // cout << pos3 << endl;

    cout << name << endl;
    cout << location << endl;
    cout << medical << endl;
    cout << fire << endl;
    cout << collapse << endl;
    cout << message << endl;
    cout << gps << endl;
}