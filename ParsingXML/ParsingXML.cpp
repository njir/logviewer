#include "stdafx.h"

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <vector>

#import <msxml3.dll>

using namespace MSXML2;
using namespace std;

//Function to throw an exception when creating COM object: 
inline void TESTHR(HRESULT _hr)
{
	if FAILED(_hr)
		throw(_hr);
}

//The following code gets all elements found in XML file,  
//and displays their names, text, and attributes:
void XMLDOMParsingNodes(char *szFileName)
{
	try	{
		//Qualify namespase explicitly to avoid Compiler Error C2872 "ambiguous symbol" during linking.
		//Now Msxml2.dll use the "MSXML2" namespace
		//(see http://support.microsoft.com/default.aspx?scid=kb;en-us;316317):
		MSXML2::IXMLDOMDocumentPtr docPtr;//pointer to DOMDocument object
		MSXML2::IXMLDOMNodeListPtr NodeListPtr;//indexed access. and iteration through the collection of nodes
		MSXML2::IXMLDOMNodePtr DOMNodePtr;//pointer to the node

		MSXML2::IXMLDOMNode *pIDOMNode = NULL;//pointer to element's node
		MSXML2::IXMLDOMNode *pIParentNode = NULL;//pointer to parent node
		MSXML2::IXMLDOMNode *pIAttrNode = NULL;//pointer to attribute node
		MSXML2::IXMLDOMNamedNodeMapPtr DOMNamedNodeMapPtr;//iteration through the collection of attribute nodes
		MSXML2::IXMLDOMNodeList *childList = NULL;//node list containing the child nodes

	    //Variable with the name of node to find: 
		BSTR strFindText = L" ";//" " means to output every node

		//Variables to store item's name, parent, text and node type:
		BSTR bstrItemText, bstrItemNode, bstrItemParent, bstrNodeType;

		//Variables to store attribute's name,type and text:	 
		BSTR bstrAttrName, bstrAttrType, bstrAttrText;

		HRESULT hResult;

		int i = 0;//loop-index variable
		int n = 0;//lines counter

		//Initialize COM Library:
		CoInitialize(NULL);

		//Create an instance of the DOMDocument object:
		docPtr.CreateInstance(__uuidof(DOMDocument30));

		// Load a document:
		_variant_t varXml(szFileName);//XML file to load
		_variant_t varResult((bool)TRUE);//result 

		varResult = docPtr->load(varXml);

		if ((bool)varResult == FALSE) {
			MessageBox(0, "Error: failed to load XML file. Check the file name.", \
				"Load XML file", MB_OK | MB_ICONWARNING);
			return;
		}

		//Collect all or selected nodes by tag name:
		NodeListPtr = docPtr->getElementsByTagName(strFindText);

		//Output the number of nodes:
		//printf("Number of nodes: %d\n", (NodeListPtr->length));

		//Output root node:
		docPtr->documentElement->get_nodeName(&bstrItemText);
		//%ls formatting is for wchar_t* parameter's type (%s for char* type):


		for (i = 0; i < (NodeListPtr->length); i++) {
			if (pIDOMNode) pIDOMNode->Release();
			NodeListPtr->get_item(i, &pIDOMNode);

			if (pIDOMNode) {
				pIDOMNode->get_nodeTypeString(&bstrNodeType);

				//We process only elements (nodes of "element" type): 
				BSTR temp = L"element";

				if (lstrcmp((LPCTSTR)bstrNodeType, (LPCTSTR)temp) == 0) {
					n++;//element node's number
					printf("\n\n%d\n", n);//element node's number
					printf("Type: %ls\n", bstrNodeType);

					pIDOMNode->get_nodeName(&bstrItemNode);
					printf("Node: %ls\n", bstrItemNode);

					pIDOMNode->get_text(&bstrItemText);
					printf("Text: %ls\n", bstrItemText);

					pIDOMNode->get_parentNode(&pIParentNode);
					pIParentNode->get_nodeName(&bstrItemParent);
					printf("Parent: %ls\n", bstrItemParent);


					pIDOMNode->get_childNodes(&childList);
					printf("Child nodes: %d\n", (childList->length));

					//Get the attributes:
					int j = 0;//loop-index variable
					long length;// number of attributes in the collection

					DOMNamedNodeMapPtr = pIDOMNode->attributes;

					hResult = DOMNamedNodeMapPtr->get_length(&length);

					if (SUCCEEDED(hResult)) {
						//Loop through the number of attributes:
						for (j = 0; j < length; j++) {
							//get attribute node:							
							DOMNamedNodeMapPtr->get_item(j, &pIAttrNode);

							pIAttrNode->get_nodeTypeString(&bstrAttrType);//type as string
							printf("\nAttribute type: %ls\n", bstrAttrType);
							//pIAttrNode->get_nodeType(&bstrAttrType);//enumerated type
							//printf("Attribute type: %d\n", bstrAttrType);
							pIAttrNode->get_nodeName(&bstrAttrName);
							printf("Attribute name: %ls\n", bstrAttrName);
							pIAttrNode->get_text(&bstrAttrText);
							printf("Attribute value: %ls\n", bstrAttrText);
						}
					}
				}
			}
		}
		//Do not forget to release interfaces:
		pIDOMNode->Release();
		pIDOMNode = NULL;
		pIParentNode->Release();
		pIParentNode = NULL;

	}

	catch (...) {
		MessageBox(NULL, ("*** Exception occurred ***"), ("Error message"), MB_OK);
	}

	CoUninitialize();
}

vector<string> GetAllFilesNames(string folder)
{
	string fullPath;
	vector<string> names;
	char searchPath[200];
	sprintf(searchPath, "%s/*.xml", folder.c_str());

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(searchPath, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				fullPath = folder + "\\" + fd.cFileName;
				names.push_back(fullPath);
				fullPath.clear();
			}

		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

int main(int argc, char* argv[])
{
	/*
	if (argc < 2) {
		cout << "Error" << endl;
		exit(1);
	}
	*/

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	vector<string> fileList = GetAllFilesNames(argv[1]);
	vector<string>::iterator itr;
	for (itr = fileList.begin(); itr != fileList.end(); itr++) {
		const char* fileName = (*itr).c_str();
		XMLDOMParsingNodes((char*)fileName);
	}

	return 0;
}


