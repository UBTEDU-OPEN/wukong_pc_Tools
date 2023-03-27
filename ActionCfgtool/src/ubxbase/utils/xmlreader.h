//Boby Thomas(06-Marh-2006)
// XMLreader.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <map>
#include "utilsdef.h"
using namespace std;

class XMLNode;

typedef map<int, XMLNode *> XMLNodeMap;
typedef XMLNodeMap::iterator XMLNodeMapIter;
typedef map<string, string> XMLStringMap;

class  UTILS_EXPORTS XMLNode
{
public:
	XMLNode(string sName, XMLNode*); // Node name
	~XMLNode(){iObjCnt--;};
	string GetElementValue(string sName); // Get element value
	XMLNode * GetFirstChild(); // returns pointer to first child node
	XMLNode * GetNextChild(); // returns pointer to first child node
	long GetChildNodeCnt();// returns the number of chid nodes for te current node.
	XMLNode* GetParentNode(){return ParentNode;};// returns pointer to parent node.
	void DeleteAll();
	XMLNode * AddChildNode(string sName);
	void AddElementVal(string sKey,string sval);
	void ResetNodeCntr(){NodeCtr =0;};
	void ResetReading();
	string GetNodeName(){return sNodeName;};
	static int GetObjectCnt(){return iObjCnt;};
private:
	string sNodeName; // Node Name
	int NodeCtr; // Node counter. Initialised to zero
	XMLNode * ParentNode; // Ponter to parent node
	XMLNodeMap XMLChildNodes;
	XMLStringMap XMLChildElements;
	static int iObjCnt;
};

class  UTILS_EXPORTS XMLRdr{
public:
	XMLRdr(string sFileName);
	~XMLRdr();
	string GetDocName();// returns DocName
	string GetRootName();// returns root element name
	XMLNode * GetRootNode();
	bool ReadFile();
	string EatupWhiteSpace(string);
	bool ProcessString(string);
	void ResetReading(){XMLRootNode->ResetReading();};
	void DeleteAll(){XMLRootNode->DeleteAll();};
private:
	string sDocName;// name of the xml doc file
	string sRootName;// XML file root node name
	XMLNode * XMLRootNode; // Root node
	XMLNode * XMLTraverse; // Root node
};

