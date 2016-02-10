#ifndef LOADMODEL_H
#define LOADMODEL_H

#include "stdafx.h"

class LoadModel
{
public:
	LoadModel(string filename);
	D3DXVECTOR4 getModelPosition(int index);
	D3DXVECTOR3 getModelNormal(int index);
	D3DXVECTOR2 getModelTexture(int index);
	int getModelDataSize();
	void ParseObject(string filename);

private:
	VertexPos4NorTex* mModelData;
	int mModelDataSize;

	void parseObj(string filename);
	void parsePos(fstream& f);
	void parseComment(fstream& f);
	void parseGroup(fstream& f);
	void parseLib(fstream& f);
	void parseTex(fstream& f);
	void parseNor(fstream& f);
	void parseSmooth(fstream& f);
	void parseFace(fstream& f);

	vector<int> mIndexPosition;
	vector<int> mIndexTexture;
	vector<int> mIndexNormal;

	vector<D3DXVECTOR3> mModelDataPosition;
	vector<D3DXVECTOR2> mModelDataTexture;
	vector<D3DXVECTOR3> mModelDataNormal;
};
#endif