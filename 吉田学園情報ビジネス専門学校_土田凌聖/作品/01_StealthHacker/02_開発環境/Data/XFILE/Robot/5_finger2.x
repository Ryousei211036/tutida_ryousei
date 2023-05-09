xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 12;
 2.91175;-5.73137;7.02978;,
 -3.05493;-5.73137;7.02978;,
 -3.05493;-6.66289;5.31411;,
 2.91175;-6.66289;5.31411;,
 -3.05493;0.61126;0.55070;,
 2.91175;0.61126;0.55070;,
 2.91175;-0.32028;-1.16496;,
 -3.05493;-0.32028;-1.16496;,
 -3.05493;-3.98064;3.04390;,
 -3.05493;-4.94059;1.34367;,
 2.91175;-3.94992;3.02722;,
 2.91175;-4.90987;1.32699;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,4,7,9;,
 4;8,9,2,1;,
 4;10,0,3,11;,
 4;10,11,6,5;,
 4;8,1,0,10;,
 4;8,10,5,4;,
 4;9,7,6,11;,
 4;9,11,3,2;;
 
 MeshMaterialList {
  1;
  10;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  10;
  0.000000;-0.878818;0.477157;,
  0.000999;-0.999082;0.042817;,
  0.000000;0.878816;-0.477161;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.000942;0.735194;0.677856;,
  -0.001783;0.914587;0.404386;,
  0.000000;0.477159;0.878817;,
  0.000949;-0.736734;-0.676182;,
  0.000000;-0.477159;-0.878817;;
  10;
  4;0,0,1,1;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;3,3,3,3;,
  4;5,6,6,5;,
  4;5,5,7,7;,
  4;8,9,9,8;,
  4;8,8,1,1;;
 }
}
