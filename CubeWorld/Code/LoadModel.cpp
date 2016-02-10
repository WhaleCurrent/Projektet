#include "LoadModel.h"

LoadModel::LoadModel(string filename)
{
	ParseObject(filename);
}

void LoadModel::ParseObject(string filename)
{
	parseObj(filename);
	mModelDataSize = mIndexPosition.size();
	mModelData = new VertexPos4NorTex[mModelDataSize];
	for(int i = 0; i < (signed)mModelDataSize; i++)
	{
		mModelData[i].pos.x = mModelDataPosition[mIndexPosition[i] - 1].x;
		mModelData[i].pos.y = mModelDataPosition[mIndexPosition[i] - 1].y;
		mModelData[i].pos.z = mModelDataPosition[mIndexPosition[i] - 1].z;
		mModelData[i].pos.w = 1.0f;

		mModelData[i].normal = mModelDataNormal[mIndexNormal[i] - 1];

		mModelData[i].texC = mModelDataTexture[mIndexTexture[i] - 1];
		mModelData[i].texC.y = 1 - mModelData[i].texC.y;
	}
}

D3DXVECTOR4 LoadModel::getModelPosition(int index)
{
	return mModelData[index].pos;
}

D3DXVECTOR3 LoadModel::getModelNormal(int index)
{
	return mModelData[index].normal;
}

D3DXVECTOR2 LoadModel::getModelTexture(int index)
{
	return mModelData[index].texC;
}

int LoadModel::getModelDataSize()
{
	return mModelDataSize;
}

void LoadModel::parseObj(string filename)
{
	fstream f(filename.data());
	string tmp;

	if(f)
	{
		while(f.eof() != true)
		{
			f >> tmp;
			if(f.eof())
				break;
			if(tmp == "v") parsePos(f);
			if(tmp == "vt") parseTex(f);
			if(tmp == "vn") parseNor(f);
			if(tmp == "#") parseComment(f);
			if(tmp == "mtllib") parseLib(f);
			if(tmp == "g") parseGroup(f);
			if(tmp == "s") parseSmooth(f);
			if(tmp == "f") parseFace(f);

		}
	}
}

void LoadModel::parsePos(fstream& f)
{
	//v (geometric vertices)
	D3DXVECTOR3 temp;
	
	f >> temp.x >> temp.y >> temp.z;

	mModelDataPosition.push_back(temp);
}

void LoadModel::parseNor(fstream& f)
{
	//vn (vertex normals)
	D3DXVECTOR3 temp;
	
	f >> temp.x >> temp.y >> temp.z;

	mModelDataNormal.push_back(temp);
}


void LoadModel::parseTex(fstream& f)
{
	//vt (texture vertices)
	D3DXVECTOR2 temp;
	
	f >> temp.x >> temp.y;

	mModelDataTexture.push_back(temp);
}


void LoadModel::parseComment(fstream& f)
{
	//# (comment)
	string tmp;

	getline(f, tmp);
}

void LoadModel::parseGroup(fstream& f)
{
	//g (group name)
	string tmp;

	getline(f, tmp);
}

void LoadModel::parseLib(fstream& f)
{
	//mtllib (material Library)
	string tmp;

	getline(f, tmp);
}

void LoadModel::parseSmooth(fstream& f)
{
	//s (smoothing group)
	string tmp;

	getline(f, tmp);
}

void LoadModel::parseFace(fstream& f)
{
	//f (face)
	int posx, posy, posz, texx, texy, texz, norx, nory, norz;
	char t;

	f >> posx >> t >> texx >> t >> norx >>
		posy >> t >> texy >> t >> nory >>
		posz >> t >> texz >> t >> norz;

	mIndexPosition.push_back(posx);
	mIndexPosition.push_back(posy);
	mIndexPosition.push_back(posz);

	mIndexTexture.push_back(texx);
	mIndexTexture.push_back(texy);
	mIndexTexture.push_back(texz);

	mIndexNormal.push_back(norx);
	mIndexNormal.push_back(nory);
	mIndexNormal.push_back(norz);
}