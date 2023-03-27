//Boby Thomas(06-Marh-2006)
// XMLreader.cpp : 
//
#include "XMLreader.h"

int XMLNode::iObjCnt;
//////////////////////////////////////////////////////////////////////////
//@f Fnct			: GetElementValue
//@r Return			: Value of element sName
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////
XMLNode::XMLNode(string sName, XMLNode * Parent)
{
	iObjCnt++; // No of objects created.
	NodeCtr = 0;
	ParentNode = Parent;
	sNodeName = sName;
}
//////////////////////////////////////////////////////////////////////////
//@f Fnct			: DeleteAll
//@r Return			: Value of element sName
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////
void XMLNode::DeleteAll()
{

	//cout<<"\nGoing to delete ::"<<sNodeName.c_str()<<"Number of childobjets:: "<<GetChildNodeCnt();
	for(XMLNodeMapIter TmpItr = XMLChildNodes.begin();TmpItr != XMLChildNodes.end(); TmpItr++)
	{
		if(TmpItr->second != NULL) 
		{
			if( (TmpItr->second->GetChildNodeCnt() != 0))
				TmpItr->second->DeleteAll();
			delete TmpItr->second;

		}
	}
	XMLChildNodes.clear(); // clear all ointers.
	XMLChildElements.clear(); // Clear all elements and vales
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: DeleteAll
//@r Return			: Value of element sName
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////
void XMLNode::ResetReading()
{

	for(XMLNodeMapIter TmpItr = XMLChildNodes.begin();TmpItr != XMLChildNodes.end(); TmpItr++)
	{
		if(TmpItr->second != NULL) 
		{
			if( (TmpItr->second->GetChildNodeCnt() != 0))
				TmpItr->second->ResetReading();
			ResetNodeCntr();

		}
	}
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: DeleteAll
//@r Return			: Value of element sName
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////
long XMLNode::GetChildNodeCnt()
{
	return XMLChildNodes.size();
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: GetElementValue
//@r Return			: Value of element sName
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////
string XMLNode::GetElementValue(string sName) // Getelement value
{
	return XMLChildElements[sName];	 
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: GetFirstChild
//@r Return			: Pointer to first chile node
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////

XMLNode * XMLNode::GetFirstChild()
{
	NodeCtr = 0;
	return XMLChildNodes[NodeCtr++];
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: GetNextChild
//@r Return			: Pointer to first next child node
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////

XMLNode * XMLNode::GetNextChild()
{

	return XMLChildNodes[NodeCtr++];	 
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: AddChildNode
//@r Return			: 
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////

XMLNode * XMLNode::AddChildNode(string sName)
{
	static int iTmp;
	//cout<<"\nAdding node::"<<sName.c_str()<<iTmp;
	XMLNode * Tmp = new XMLNode(sName,this);
	XMLChildNodes[NodeCtr++] = Tmp;
	iTmp++;
	
	return Tmp;
}
//////////////////////////////////////////////////////////////////////////
//@f Fnct			: XMLNode
//@r Return			: none
//@f Sideeffects	: none
//@a Boby thomas
/////////////////XMLRdr/////////////////////////////////////////////////////////

void XMLNode::AddElementVal(string sKey,string sval)
{
	XMLChildElements[sKey] = sval;
	return;
}



//////////////////////////////////////////////////////////////////////////
//@f Fnct			: XMLRdr constructor
//@r Return			: none
//@f Sideeffects	: none
//@a Boby thomas
/////////////////XMLRdr/////////////////////////////////////////////////////////

XMLRdr::XMLRdr(string sFileName)
{
	sDocName = sFileName;
	XMLRootNode = NULL;
	XMLTraverse = NULL;
	if(ReadFile() == false)
		cout<<"Unable to open file";
	else
		cout<<"Fle opened successfuly";

	return;	 
}
//////////////////////////////////////////////////////////////////////////
//@f Fnct			: XMLRdr constructor
//@r Return			: none
//@f Sideeffects	: none
//@a Boby thomas
/////////////////XMLRdr/////////////////////////////////////////////////////////

bool XMLRdr::ReadFile()
{
	FILE *fp = fopen(sDocName.c_str(),"r+");
	if(fp == NULL)
		return false;
	char buf[1000];
	while(fgets(buf, 1000, fp))
	{
		string sTmp = buf;
		sTmp = EatupWhiteSpace(sTmp);
		if(sTmp.length() && ProcessString(sTmp) == false)
		{
			XMLRootNode->DeleteAll();
			return false;
		}
	}
	if(XMLTraverse != NULL)
		return false;
	fclose(fp);
	return true;
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: XMLRdr constructor
//@r Return			: none
//@f Sideeffects	: none
//@a Boby thomas
/////////////////XMLRdr/////////////////////////////////////////////////////////
string XMLRdr::EatupWhiteSpace(string sInput)
{
	int iStart = sInput.find_first_not_of(" \t\n");
	int iEnd = sInput.find_last_not_of(" \t\n");
	if(iStart == -1) // No non-spaces
		return "";

	return std::string(sInput, iStart, iEnd - iStart + 1);
}
//////////////////////////////////////////////////////////////////////////
//@f Fnct			: XMLRdr constructor
//@r Return			: none
//@f Sideeffects	: none
//@a Boby thomas
/////////////////XMLRdr/////////////////////////////////////////////////////////
bool XMLRdr::ProcessString(string sInput)
{
	
	if(sInput[0] != '<')
		return false;
	switch(sInput[1])
	{
	case '?':
	case '!':
		return true;
		break;
	case '/':
		if(XMLTraverse == NULL)
			return false;
		else
		{
			XMLTraverse->ResetNodeCntr();
			XMLTraverse = XMLTraverse->GetParentNode();
		}
		break;
	default:
		{
			sInput = sInput.substr(1);
			int iTmp = sInput.find_first_of("<");
			
			if(iTmp != -1)//not a node .But an element 
			{
				iTmp = sInput.find_first_of(" \t>"); //ignore atributes. if any.
				string sKey = sInput.substr(0,iTmp);
				iTmp = sInput.find_first_of(">");
				sInput = sInput.substr(iTmp+1);
				iTmp = sInput.find_first_of("<");
				string sVal = sInput.substr(0,iTmp);
				XMLTraverse->AddElementVal(sKey,sVal);
				//cout<<"KEY ="<<sKey.c_str()<<"\t\t\tVAL ="<<sVal.c_str()<<"\n";
			}
			else
			{
				iTmp = sInput.find_first_of(" \t>");
				string sNodeName = sInput.substr(0,iTmp);
				if(XMLRootNode == NULL)
				{
					XMLRootNode = new XMLNode(sNodeName.c_str(),NULL);
					XMLTraverse = XMLRootNode;
				}
				else
				{
					XMLTraverse = XMLTraverse->AddChildNode(sNodeName);
				}

			}
			break;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: XMLRdr destructor
//@r Return			: none
//@f Sideeffects	: none
//@a Boby thomas
/////////////////XMLRdr/////////////////////////////////////////////////////////

XMLRdr::~XMLRdr()
{
	
	if( (XMLRootNode == NULL) || (XMLNode::GetObjectCnt() == 0) )
		return;
	DeleteAll();
	if(XMLRootNode != NULL)
		delete XMLRootNode;
	
	if(!XMLNode::GetObjectCnt())
		cout<<"\nAll Node Objects deleted";	
	return;	 
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: GetDocName
//@r Return			: Document name
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////
string XMLRdr::GetDocName()
{
	return sDocName;	 
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: GetDocName
//@r Return			: Root node name
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////
string XMLRdr::GetRootName()
{
	return sRootName;	 
}

//////////////////////////////////////////////////////////////////////////
//@f Fnct			: GetRootNode
//@r Return			: Pointer to root node
//@f Sideeffects	: none
//@a Boby thomas
//////////////////////////////////////////////////////////////////////////
XMLNode * XMLRdr::GetRootNode()
{
	return XMLRootNode;	 
}






