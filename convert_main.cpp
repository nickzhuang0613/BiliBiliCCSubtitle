#include <iostream>
#include <cstdio>
#include <string>
#include <json/json.h>
#include <fstream>
#include <regex>
using namespace std;
void usage(string program){
    cout << "Usage: " << program << " [-o outputfile] inputfile" << endl;
}
string time_convert(string raw)
{
    int h=0,m=0,s=0,ms=0;
    string s1,s2;
    bool isS1=true;
    for(auto c:raw)
    {
        if(isS1 && c!='.')
            s1+=c;
        else if(c=='.')
            isS1=false;
        else
            s2+=c;
    }
    while (s2[s2.length()-1]=='0')
        s2=s2.substr(0,s2.length()-1);
    if(s2=="")
        s2="0";
    ms=std::stoi(s2);
    s=std::stoi(s1);
    while(s>=60)
    {
        m++;
        s-=60;
    }
    while(m>=60)
    {
        h++;
        m-=60;
    }
    char tmp[50];
    if(ms<100)
        sprintf(tmp,"%02d:%02d:%02d,%d0",h,m,s,ms);
    else
        sprintf(tmp,"%02d:%02d:%02d,%d",h,m,s,ms);
    return string(tmp);
}
int main(int argc,char **argv) {
    cout << "Bilibili JSON CC subtitle to srt format translator by 晴酱(alias Nathanli)" << endl;
    if(argc < 2 || argc > 4)
    {
        cerr << "Invaild argument" << endl;
        usage(argv[0]);
        return -1;
    }
    string inputfile=argv[1];
    string outputfile=argv[1];
    if(inputfile=="-o")
    {
        if(argc !=4)
        {
            cerr << "Invaild argument" << endl;
            usage(argv[0]);
            return -1;
        }
        inputfile=argv[3];
        outputfile=argv[2];
    } else
        outputfile=regex_replace(outputfile,regex(".json$"),".srt");
    cout << "Input file:" << inputfile << endl << "Output file:" << outputfile << endl;
    Json::Reader reader;
    Json::Value root;
    ifstream input;
    input.open(inputfile);
    ofstream output(outputfile,ios::out|ios::trunc);
    if(!input.is_open() || !output.is_open())
    {
        cerr << "Failed to open";
        if(!input.is_open()) cerr <<" input";
        else input.close();
        if(!output.is_open()) cerr << " output";
        else output.close();
        cerr << " file" << endl;
        return -1;
    }
    string input_json,tmp;
    while (getline(input,tmp))
        input_json+=tmp+"\n";
    if (!reader.parse(input_json, root))
    {
        cerr << "Failed to parse json document" << endl;
        return -1;
    }
    if(root["body"].isNull() || !root["body"].isArray())
    {
        cerr << "Wrong CC format" << endl;
        return -1;
    }
    long long i=0;
    auto write_srt=[&output,&i](string from,string to,string content) mutable->void{
        regex r1("\\n");
        regex r2("\\n\\r");
        content=regex_replace(content,r2,"\\N");
        content=regex_replace(content,r1,"\\N");

        output << i << endl;
        output << time_convert(from) << " --> " << time_convert(to) << endl;
        output << content << endl << endl;
        i++;
    };
    Json::Value body=root["body"];
    for(auto i:body)
    {
        if(i["from"].isNull() || !(i["from"].isDouble() || i["from"].isInt()))
        {
            cerr << "Wrong CC format" << endl;
            return -1;
        }
        if(i["to"].isNull() || !(i["to"].isDouble() || i["to"].isInt()))
        {
            cerr << "Wrong CC format" << endl;
            return -1;
        }
        if(i["content"].isNull() || !i["content"].isString())
        {
            cerr << "Wrong CC format" << endl;
            return -1;
        }
        write_srt(to_string(i["from"].asDouble()),to_string(i["to"].asDouble()),i["content"].asString());
    }
    input.close();
    output.close();
}
