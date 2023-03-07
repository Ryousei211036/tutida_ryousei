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
 262;
 -0.11064;0.36731;6.29273;,
 -12.94283;1.56997;6.29274;,
 -12.94283;1.56997;-7.70727;,
 -0.11064;0.36731;-7.70726;,
 -18.25811;7.36731;6.29274;,
 -18.25811;7.36731;-7.70727;,
 -12.94283;13.64695;6.29274;,
 -12.94283;13.64694;-7.70727;,
 -0.11063;14.36730;6.29273;,
 -0.11063;14.36730;-7.70727;,
 12.72152;13.64694;6.29273;,
 12.72152;13.64694;-7.70727;,
 18.03681;7.36731;6.29273;,
 18.03681;7.36731;-7.70727;,
 12.72152;1.56998;6.29273;,
 12.72152;1.56997;-7.70727;,
 -0.11064;7.36731;6.29273;,
 -0.11064;7.36731;-7.70727;,
 7.01034;10.33865;7.75493;,
 6.22686;10.11825;8.53842;,
 7.01034;10.11825;8.86294;,
 5.90233;10.11825;7.75493;,
 6.22686;10.11825;6.97145;,
 7.01034;10.11825;6.64693;,
 7.79381;10.11825;6.97145;,
 8.11835;10.11825;7.75493;,
 7.79381;10.11825;8.53842;,
 5.56265;9.49061;9.20262;,
 7.01034;9.49061;9.80228;,
 4.96300;9.49061;7.75493;,
 5.56265;9.49061;6.30725;,
 7.01034;9.49061;5.70760;,
 8.45802;9.49061;6.30725;,
 9.05768;9.49061;7.75493;,
 8.45802;9.49061;9.20262;,
 5.11885;8.55129;9.64643;,
 7.01034;8.55129;10.42992;,
 4.33536;8.55129;7.75493;,
 5.11885;8.55129;5.86344;,
 7.01034;8.55129;5.07996;,
 8.90183;8.55129;5.86344;,
 9.68531;8.55129;7.75493;,
 8.90183;8.55129;9.64643;,
 4.96300;7.44327;9.80228;,
 7.01034;7.44327;10.65030;,
 4.11496;7.44327;7.75493;,
 4.96300;7.44327;5.70760;,
 7.01034;7.44327;4.85956;,
 9.05768;7.44327;5.70760;,
 9.90571;7.44327;7.75493;,
 9.05768;7.44327;9.80228;,
 5.11885;6.33526;9.64643;,
 7.01034;6.33526;10.42992;,
 4.33536;6.33526;7.75493;,
 5.11885;6.33526;5.86344;,
 7.01034;6.33526;5.07996;,
 8.90183;6.33526;5.86344;,
 9.68531;6.33526;7.75493;,
 8.90183;6.33526;9.64643;,
 5.56265;5.39594;9.20262;,
 7.01034;5.39594;9.80228;,
 4.96300;5.39594;7.75493;,
 5.56265;5.39594;6.30725;,
 7.01034;5.39594;5.70760;,
 8.45802;5.39594;6.30725;,
 9.05768;5.39594;7.75493;,
 8.45802;5.39594;9.20262;,
 6.22686;4.76830;8.53843;,
 7.01034;4.76830;8.86294;,
 5.90233;4.76830;7.75493;,
 6.22686;4.76830;6.97145;,
 7.01034;4.76830;6.64693;,
 7.79381;4.76830;6.97145;,
 8.11835;4.76830;7.75493;,
 7.79381;4.76830;8.53842;,
 7.01034;4.54790;7.75493;,
 -7.19136;10.33865;7.75493;,
 -7.19136;10.11825;8.86294;,
 -6.40788;10.11825;8.53842;,
 -6.08335;10.11825;7.75493;,
 -6.40788;10.11825;6.97145;,
 -7.19136;10.11825;6.64693;,
 -7.97484;10.11825;6.97145;,
 -8.29937;10.11825;7.75493;,
 -7.97484;10.11825;8.53842;,
 -7.19136;9.49061;9.80228;,
 -5.74367;9.49061;9.20262;,
 -5.14402;9.49061;7.75493;,
 -5.74367;9.49061;6.30725;,
 -7.19136;9.49061;5.70760;,
 -8.63904;9.49061;6.30725;,
 -9.23870;9.49061;7.75493;,
 -8.63904;9.49061;9.20262;,
 -7.19136;8.55129;10.42992;,
 -5.29987;8.55129;9.64643;,
 -4.51638;8.55129;7.75493;,
 -5.29987;8.55129;5.86344;,
 -7.19136;8.55129;5.07996;,
 -9.08285;8.55129;5.86344;,
 -9.86634;8.55129;7.75494;,
 -9.08285;8.55129;9.64643;,
 -7.19136;7.44327;10.65031;,
 -5.14402;7.44327;9.80228;,
 -4.29598;7.44327;7.75493;,
 -5.14402;7.44327;5.70760;,
 -7.19136;7.44327;4.85956;,
 -9.23870;7.44327;5.70760;,
 -10.08673;7.44327;7.75494;,
 -9.23870;7.44327;9.80228;,
 -7.19136;6.33526;10.42992;,
 -5.29987;6.33526;9.64643;,
 -4.51638;6.33526;7.75493;,
 -5.29987;6.33526;5.86344;,
 -7.19136;6.33526;5.07996;,
 -9.08285;6.33526;5.86344;,
 -9.86634;6.33526;7.75494;,
 -9.08285;6.33526;9.64643;,
 -7.19136;5.39594;9.80228;,
 -5.74367;5.39594;9.20262;,
 -5.14402;5.39594;7.75493;,
 -5.74367;5.39594;6.30725;,
 -7.19136;5.39594;5.70760;,
 -8.63904;5.39594;6.30725;,
 -9.23870;5.39594;7.75493;,
 -8.63904;5.39594;9.20262;,
 -7.19136;4.76830;8.86294;,
 -6.40788;4.76830;8.53843;,
 -6.08335;4.76830;7.75493;,
 -6.40788;4.76830;6.97145;,
 -7.19136;4.76830;6.64693;,
 -7.97484;4.76830;6.97145;,
 -8.29937;4.76830;7.75493;,
 -7.97484;4.76830;8.53842;,
 -7.19136;4.54790;7.75493;,
 2.09163;7.40584;6.46221;,
 1.91243;7.40207;9.36970;,
 3.39846;3.81459;9.15928;,
 3.52518;3.94507;6.25923;,
 1.47982;7.40050;10.57402;,
 3.09256;3.50713;10.34566;,
 1.04721;7.40207;9.36970;,
 2.78666;3.20281;9.12339;,
 0.86801;7.40584;6.46221;,
 2.65995;3.07988;6.20848;,
 1.04721;7.40961;3.55472;,
 2.78666;3.21036;3.30842;,
 1.47982;7.41118;2.35040;,
 3.09256;3.51781;2.12205;,
 1.91243;7.40961;3.55472;,
 3.39846;3.82214;3.34430;,
 6.98608;2.32861;9.07214;,
 6.98608;2.51157;6.17516;,
 6.98608;1.89444;10.25109;,
 6.98608;1.46342;9.02137;,
 6.98608;1.28801;6.10337;,
 6.98608;1.47097;3.20640;,
 6.98608;1.90513;2.02746;,
 6.98608;2.33616;3.25715;,
 10.57369;3.81459;9.15928;,
 10.44698;3.94507;6.25923;,
 10.87959;3.50713;10.34566;,
 11.18550;3.20281;9.12339;,
 11.31221;3.07988;6.20848;,
 11.18550;3.21036;3.30842;,
 10.87959;3.51781;2.12205;,
 10.57369;3.82214;3.34430;,
 12.05972;7.40207;9.36970;,
 11.88053;7.40584;6.46221;,
 12.49233;7.40050;10.57402;,
 12.92495;7.40207;9.36970;,
 13.10415;7.40584;6.46221;,
 12.92495;7.40961;3.55472;,
 12.49233;7.41118;2.35040;,
 12.05972;7.40961;3.55472;,
 10.57369;10.98954;9.58011;,
 10.44698;10.86660;6.66520;,
 10.87959;11.29386;10.80237;,
 11.18550;11.60132;9.61600;,
 11.31221;11.73180;6.71595;,
 11.18550;11.60887;3.80103;,
 10.87959;11.30454;2.57876;,
 10.57369;10.99709;3.76514;,
 6.98608;12.47552;9.66727;,
 6.98608;12.30011;6.74926;,
 6.98608;12.90655;10.89696;,
 6.98608;13.34070;9.71803;,
 6.98608;13.52367;6.82105;,
 6.98608;13.34826;3.90304;,
 6.98608;12.91723;2.67334;,
 6.98608;12.48306;3.85229;,
 3.39846;10.98954;9.58011;,
 3.52518;10.86661;6.66520;,
 3.09256;11.29386;10.80237;,
 2.78666;11.60132;9.61600;,
 2.65995;11.73180;6.71595;,
 2.78666;11.60887;3.80103;,
 3.09256;11.30454;2.57876;,
 3.39846;10.99709;3.76514;,
 -12.05189;7.40584;6.46222;,
 -12.23108;7.40207;9.36970;,
 -10.74504;3.81459;9.15928;,
 -10.61833;3.94507;6.25924;,
 -12.66369;7.40050;10.57403;,
 -11.05094;3.50713;10.34566;,
 -13.09630;7.40207;9.36970;,
 -11.35685;3.20281;9.12339;,
 -13.27549;7.40584;6.46222;,
 -11.48356;3.07988;6.20849;,
 -13.09630;7.40961;3.55472;,
 -11.35685;3.21036;3.30843;,
 -12.66369;7.41118;2.35041;,
 -11.05094;3.51781;2.12206;,
 -12.23108;7.40961;3.55472;,
 -10.74504;3.82214;3.34431;,
 -7.15743;2.32861;9.07214;,
 -7.15743;2.51157;6.17516;,
 -7.15743;1.89444;10.25109;,
 -7.15743;1.46342;9.02137;,
 -7.15743;1.28801;6.10337;,
 -7.15743;1.47097;3.20640;,
 -7.15743;1.90513;2.02746;,
 -7.15743;2.33616;3.25715;,
 -3.56982;3.81459;9.15928;,
 -3.69653;3.94507;6.25923;,
 -3.26392;3.50713;10.34566;,
 -2.95801;3.20281;9.12339;,
 -2.83130;3.07988;6.20848;,
 -2.95801;3.21036;3.30842;,
 -3.26392;3.51781;2.12205;,
 -3.56982;3.82214;3.34430;,
 -2.08379;7.40207;9.36970;,
 -2.26297;7.40584;6.46221;,
 -1.65117;7.40050;10.57402;,
 -1.21856;7.40207;9.36970;,
 -1.03936;7.40584;6.46221;,
 -1.21856;7.40961;3.55472;,
 -1.65117;7.41118;2.35040;,
 -2.08379;7.40961;3.55472;,
 -3.56982;10.98954;9.58011;,
 -3.69653;10.86660;6.66520;,
 -3.26392;11.29386;10.80237;,
 -2.95802;11.60132;9.61600;,
 -2.83130;11.73180;6.71595;,
 -2.95802;11.60887;3.80103;,
 -3.26392;11.30454;2.57876;,
 -3.56982;10.99709;3.76514;,
 -7.15743;12.47552;9.66727;,
 -7.15743;12.30011;6.74926;,
 -7.15743;12.90655;10.89697;,
 -7.15743;13.34070;9.71803;,
 -7.15743;13.52367;6.82105;,
 -7.15743;13.34826;3.90304;,
 -7.15743;12.91723;2.67334;,
 -7.15743;12.48306;3.85229;,
 -10.74504;10.98954;9.58011;,
 -10.61833;10.86661;6.66520;,
 -11.05094;11.29386;10.80238;,
 -11.35685;11.60132;9.61600;,
 -11.48356;11.73180;6.71595;,
 -11.35685;11.60887;3.80104;,
 -11.05094;11.30454;2.57876;,
 -10.74504;10.99709;3.76514;;
 
 280;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 3;16,1,0;,
 3;16,4,1;,
 3;16,6,4;,
 3;16,8,6;,
 3;16,10,8;,
 3;16,12,10;,
 3;16,14,12;,
 3;16,0,14;,
 3;17,3,2;,
 3;17,2,5;,
 3;17,5,7;,
 3;17,7,9;,
 3;17,9,11;,
 3;17,11,13;,
 3;17,13,15;,
 3;17,15,3;,
 3;18,19,20;,
 3;18,21,19;,
 3;18,22,21;,
 3;18,23,22;,
 3;18,24,23;,
 3;18,25,24;,
 3;18,26,25;,
 3;18,20,26;,
 4;20,19,27,28;,
 4;19,21,29,27;,
 4;21,22,30,29;,
 4;22,23,31,30;,
 4;23,24,32,31;,
 4;24,25,33,32;,
 4;25,26,34,33;,
 4;26,20,28,34;,
 4;28,27,35,36;,
 4;27,29,37,35;,
 4;29,30,38,37;,
 4;30,31,39,38;,
 4;31,32,40,39;,
 4;32,33,41,40;,
 4;33,34,42,41;,
 4;34,28,36,42;,
 4;36,35,43,44;,
 4;35,37,45,43;,
 4;37,38,46,45;,
 4;38,39,47,46;,
 4;39,40,48,47;,
 4;40,41,49,48;,
 4;41,42,50,49;,
 4;42,36,44,50;,
 4;44,43,51,52;,
 4;43,45,53,51;,
 4;45,46,54,53;,
 4;46,47,55,54;,
 4;47,48,56,55;,
 4;48,49,57,56;,
 4;49,50,58,57;,
 4;50,44,52,58;,
 4;52,51,59,60;,
 4;51,53,61,59;,
 4;53,54,62,61;,
 4;54,55,63,62;,
 4;55,56,64,63;,
 4;56,57,65,64;,
 4;57,58,66,65;,
 4;58,52,60,66;,
 4;60,59,67,68;,
 4;59,61,69,67;,
 4;61,62,70,69;,
 4;62,63,71,70;,
 4;63,64,72,71;,
 4;64,65,73,72;,
 4;65,66,74,73;,
 4;66,60,68,74;,
 3;68,67,75;,
 3;67,69,75;,
 3;69,70,75;,
 3;70,71,75;,
 3;71,72,75;,
 3;72,73,75;,
 3;73,74,75;,
 3;74,68,75;,
 3;76,77,78;,
 3;76,78,79;,
 3;76,79,80;,
 3;76,80,81;,
 3;76,81,82;,
 3;76,82,83;,
 3;76,83,84;,
 3;76,84,77;,
 4;77,85,86,78;,
 4;78,86,87,79;,
 4;79,87,88,80;,
 4;80,88,89,81;,
 4;81,89,90,82;,
 4;82,90,91,83;,
 4;83,91,92,84;,
 4;84,92,85,77;,
 4;85,93,94,86;,
 4;86,94,95,87;,
 4;87,95,96,88;,
 4;88,96,97,89;,
 4;89,97,98,90;,
 4;90,98,99,91;,
 4;91,99,100,92;,
 4;92,100,93,85;,
 4;93,101,102,94;,
 4;94,102,103,95;,
 4;95,103,104,96;,
 4;96,104,105,97;,
 4;97,105,106,98;,
 4;98,106,107,99;,
 4;99,107,108,100;,
 4;100,108,101,93;,
 4;101,109,110,102;,
 4;102,110,111,103;,
 4;103,111,112,104;,
 4;104,112,113,105;,
 4;105,113,114,106;,
 4;106,114,115,107;,
 4;107,115,116,108;,
 4;108,116,109,101;,
 4;109,117,118,110;,
 4;110,118,119,111;,
 4;111,119,120,112;,
 4;112,120,121,113;,
 4;113,121,122,114;,
 4;114,122,123,115;,
 4;115,123,124,116;,
 4;116,124,117,109;,
 4;117,125,126,118;,
 4;118,126,127,119;,
 4;119,127,128,120;,
 4;120,128,129,121;,
 4;121,129,130,122;,
 4;122,130,131,123;,
 4;123,131,132,124;,
 4;124,132,125,117;,
 3;125,133,126;,
 3;126,133,127;,
 3;127,133,128;,
 3;128,133,129;,
 3;129,133,130;,
 3;130,133,131;,
 3;131,133,132;,
 3;132,133,125;,
 4;134,135,136,137;,
 4;135,138,139,136;,
 4;138,140,141,139;,
 4;140,142,143,141;,
 4;142,144,145,143;,
 4;144,146,147,145;,
 4;146,148,149,147;,
 4;148,134,137,149;,
 4;137,136,150,151;,
 4;136,139,152,150;,
 4;139,141,153,152;,
 4;141,143,154,153;,
 4;143,145,155,154;,
 4;145,147,156,155;,
 4;147,149,157,156;,
 4;149,137,151,157;,
 4;151,150,158,159;,
 4;150,152,160,158;,
 4;152,153,161,160;,
 4;153,154,162,161;,
 4;154,155,163,162;,
 4;155,156,164,163;,
 4;156,157,165,164;,
 4;157,151,159,165;,
 4;159,158,166,167;,
 4;158,160,168,166;,
 4;160,161,169,168;,
 4;161,162,170,169;,
 4;162,163,171,170;,
 4;163,164,172,171;,
 4;164,165,173,172;,
 4;165,159,167,173;,
 4;167,166,174,175;,
 4;166,168,176,174;,
 4;168,169,177,176;,
 4;169,170,178,177;,
 4;170,171,179,178;,
 4;171,172,180,179;,
 4;172,173,181,180;,
 4;173,167,175,181;,
 4;175,174,182,183;,
 4;174,176,184,182;,
 4;176,177,185,184;,
 4;177,178,186,185;,
 4;178,179,187,186;,
 4;179,180,188,187;,
 4;180,181,189,188;,
 4;181,175,183,189;,
 4;183,182,190,191;,
 4;182,184,192,190;,
 4;184,185,193,192;,
 4;185,186,194,193;,
 4;186,187,195,194;,
 4;187,188,196,195;,
 4;188,189,197,196;,
 4;189,183,191,197;,
 4;191,190,135,134;,
 4;190,192,138,135;,
 4;192,193,140,138;,
 4;193,194,142,140;,
 4;194,195,144,142;,
 4;195,196,146,144;,
 4;196,197,148,146;,
 4;197,191,134,148;,
 4;198,199,200,201;,
 4;199,202,203,200;,
 4;202,204,205,203;,
 4;204,206,207,205;,
 4;206,208,209,207;,
 4;208,210,211,209;,
 4;210,212,213,211;,
 4;212,198,201,213;,
 4;201,200,214,215;,
 4;200,203,216,214;,
 4;203,205,217,216;,
 4;205,207,218,217;,
 4;207,209,219,218;,
 4;209,211,220,219;,
 4;211,213,221,220;,
 4;213,201,215,221;,
 4;215,214,222,223;,
 4;214,216,224,222;,
 4;216,217,225,224;,
 4;217,218,226,225;,
 4;218,219,227,226;,
 4;219,220,228,227;,
 4;220,221,229,228;,
 4;221,215,223,229;,
 4;223,222,230,231;,
 4;222,224,232,230;,
 4;224,225,233,232;,
 4;225,226,234,233;,
 4;226,227,235,234;,
 4;227,228,236,235;,
 4;228,229,237,236;,
 4;229,223,231,237;,
 4;231,230,238,239;,
 4;230,232,240,238;,
 4;232,233,241,240;,
 4;233,234,242,241;,
 4;234,235,243,242;,
 4;235,236,244,243;,
 4;236,237,245,244;,
 4;237,231,239,245;,
 4;239,238,246,247;,
 4;238,240,248,246;,
 4;240,241,249,248;,
 4;241,242,250,249;,
 4;242,243,251,250;,
 4;243,244,252,251;,
 4;244,245,253,252;,
 4;245,239,247,253;,
 4;247,246,254,255;,
 4;246,248,256,254;,
 4;248,249,257,256;,
 4;249,250,258,257;,
 4;250,251,259,258;,
 4;251,252,260,259;,
 4;252,253,261,260;,
 4;253,247,255,261;,
 4;255,254,199,198;,
 4;254,256,202,199;,
 4;256,257,204,202;,
 4;257,258,206,204;,
 4;258,259,208,206;,
 4;259,260,210,208;,
 4;260,261,212,210;,
 4;261,255,198,212;;
 
 MeshMaterialList {
  4;
  280;
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.282400;0.800000;0.000000;1.000000;;
   5.000000;
   1.000000;1.000000;1.000000;;
   0.353000;1.000000;0.000000;;
  }
  Material {
   0.564800;0.668000;0.000000;1.000000;;
   11.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  298;
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;0.000000;,
  -0.444933;-0.895564;0.000000;,
  -0.737088;-0.675797;0.000000;,
  -0.445943;0.895061;-0.000000;,
  0.000000;1.000000;-0.000000;,
  0.445943;0.895061;0.000000;,
  0.763281;0.646067;0.000000;,
  0.444933;-0.895564;0.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.763282;0.646066;-0.000000;,
  0.737087;-0.675798;0.000000;,
  0.000001;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000001;0.000000;1.000000;,
  0.000001;-0.000000;1.000000;,
  0.000000;-0.000000;1.000000;,
  0.000000;-0.000000;1.000000;,
  -0.000000;-0.000001;-1.000000;,
  0.000000;-0.000001;-1.000000;,
  0.000000;0.000000;-1.000000;,
  -0.000000;-0.000001;-1.000000;,
  0.000000;1.000000;-0.000001;,
  0.000001;0.924735;0.380612;,
  -0.269132;0.924735;0.269133;,
  -0.380612;0.924735;-0.000001;,
  -0.269132;0.924735;-0.269135;,
  0.000001;0.924734;-0.380614;,
  0.269133;0.924734;-0.269136;,
  0.380611;0.924735;-0.000001;,
  0.269133;0.924735;0.269135;,
  0.000001;0.709233;0.704974;,
  -0.498494;0.709232;0.498492;,
  -0.704976;0.709231;-0.000001;,
  -0.498492;0.709231;-0.498495;,
  0.000001;0.709231;-0.704976;,
  0.498492;0.709231;-0.498495;,
  0.704975;0.709232;-0.000001;,
  0.498493;0.709231;0.498493;,
  0.000000;0.384551;0.923104;,
  -0.652733;0.384552;0.652733;,
  -0.923103;0.384553;-0.000001;,
  -0.652732;0.384552;-0.652734;,
  0.000000;0.384553;-0.923103;,
  0.652732;0.384553;-0.652733;,
  0.923103;0.384553;-0.000001;,
  0.652734;0.384552;0.652732;,
  -0.000000;-0.000001;1.000000;,
  -0.707106;-0.000001;0.707108;,
  -1.000000;-0.000001;-0.000001;,
  -0.707106;-0.000001;-0.707108;,
  0.000000;-0.000001;-1.000000;,
  0.707107;-0.000001;-0.707106;,
  1.000000;-0.000001;-0.000001;,
  0.707107;-0.000001;0.707106;,
  0.000000;-0.384552;0.923104;,
  -0.652733;-0.384552;0.652732;,
  -0.923102;-0.384554;-0.000001;,
  -0.652731;-0.384553;-0.652733;,
  0.000000;-0.384553;-0.923103;,
  0.652732;-0.384553;-0.652732;,
  0.923103;-0.384554;-0.000001;,
  0.652733;-0.384553;0.652731;,
  0.000001;-0.709232;0.704975;,
  -0.498493;-0.709231;0.498493;,
  -0.704976;-0.709231;-0.000001;,
  -0.498492;-0.709231;-0.498495;,
  0.000001;-0.709231;-0.704976;,
  0.498492;-0.709231;-0.498495;,
  0.704975;-0.709232;-0.000001;,
  0.498493;-0.709232;0.498493;,
  0.000002;-0.924735;0.380613;,
  -0.269132;-0.924735;0.269134;,
  -0.380612;-0.924735;-0.000001;,
  -0.269132;-0.924735;-0.269135;,
  0.000001;-0.924734;-0.380614;,
  0.269132;-0.924734;-0.269136;,
  0.380611;-0.924735;-0.000001;,
  0.269132;-0.924735;0.269134;,
  0.000001;-1.000000;-0.000001;,
  0.000000;1.000000;-0.000001;,
  -0.000000;0.924735;0.380612;,
  0.269133;0.924735;0.269133;,
  0.380612;0.924735;-0.000001;,
  0.269132;0.924735;-0.269135;,
  -0.000000;0.924734;-0.380614;,
  -0.269133;0.924734;-0.269136;,
  -0.380613;0.924735;-0.000001;,
  -0.269133;0.924735;0.269133;,
  -0.000000;0.709233;0.704975;,
  0.498494;0.709231;0.498493;,
  0.704976;0.709231;-0.000001;,
  0.498492;0.709231;-0.498495;,
  -0.000000;0.709231;-0.704976;,
  -0.498492;0.709231;-0.498495;,
  -0.704975;0.709232;-0.000001;,
  -0.498493;0.709232;0.498492;,
  -0.000000;0.384553;0.923103;,
  0.652733;0.384552;0.652732;,
  0.923103;0.384553;-0.000001;,
  0.652732;0.384552;-0.652734;,
  -0.000000;0.384553;-0.923103;,
  -0.652732;0.384552;-0.652733;,
  -0.923103;0.384553;0.000000;,
  -0.652732;0.384553;0.652732;,
  0.000000;-0.000001;1.000000;,
  0.707107;-0.000001;0.707107;,
  1.000000;-0.000001;-0.000001;,
  0.707106;-0.000001;-0.707108;,
  0.000000;-0.000001;-1.000000;,
  -0.707107;-0.000001;-0.707106;,
  -1.000000;-0.000001;0.000000;,
  -0.707107;-0.000001;0.707107;,
  -0.000000;-0.384554;0.923103;,
  0.652733;-0.384553;0.652732;,
  0.923102;-0.384554;-0.000001;,
  0.652731;-0.384553;-0.652733;,
  -0.000000;-0.384553;-0.923103;,
  -0.652732;-0.384553;-0.652733;,
  -0.923103;-0.384554;-0.000000;,
  -0.652732;-0.384554;0.652732;,
  -0.000001;-0.709232;0.704975;,
  0.498493;-0.709232;0.498492;,
  0.704976;-0.709231;-0.000001;,
  0.498492;-0.709231;-0.498494;,
  -0.000001;-0.709231;-0.704976;,
  -0.498492;-0.709231;-0.498495;,
  -0.704975;-0.709232;-0.000001;,
  -0.498493;-0.709232;0.498493;,
  -0.000002;-0.924735;0.380612;,
  0.269132;-0.924736;0.269133;,
  0.380612;-0.924735;-0.000001;,
  0.269132;-0.924735;-0.269134;,
  -0.000000;-0.924734;-0.380614;,
  -0.269133;-0.924734;-0.269136;,
  -0.380612;-0.924735;-0.000001;,
  -0.269133;-0.924735;0.269134;,
  -0.000000;-1.000000;-0.000001;,
  1.000000;0.000000;0.000000;,
  0.979258;-0.010313;0.202352;,
  0.940990;-0.017268;0.337994;,
  -0.979259;-0.010313;0.202352;,
  -1.000000;0.000000;0.000000;,
  -0.979259;0.010313;-0.202352;,
  -0.940990;0.017268;-0.337995;,
  0.979258;0.010313;-0.202353;,
  0.707118;0.707094;0.001049;,
  0.697196;0.686777;0.205557;,
  0.672799;0.655344;0.343316;,
  -0.687808;-0.698017;0.199233;,
  -0.707118;-0.707095;-0.001049;,
  -0.697196;-0.686778;-0.205555;,
  -0.672799;-0.655345;-0.343314;,
  0.687807;0.698016;-0.199238;,
  -0.000000;0.999999;0.001484;,
  -0.000000;0.978360;0.206911;,
  -0.000000;0.938390;0.345577;,
  0.000000;-0.980209;0.197965;,
  0.000000;-0.999999;-0.001484;,
  0.000000;-0.978360;-0.206909;,
  0.000000;-0.938392;-0.345573;,
  -0.000000;0.980208;-0.197971;,
  -0.707119;0.707094;0.001049;,
  -0.697197;0.686777;0.205556;,
  -0.672799;0.655343;0.343316;,
  0.687807;-0.698016;0.199235;,
  0.707118;-0.707095;-0.001049;,
  0.697196;-0.686778;-0.205558;,
  0.672798;-0.655344;-0.343318;,
  -0.687807;0.698016;-0.199237;,
  -1.000000;-0.000000;0.000000;,
  -0.979259;-0.010313;0.202351;,
  -0.940990;-0.017268;0.337994;,
  0.979258;-0.010313;0.202355;,
  1.000000;-0.000000;0.000000;,
  0.979258;0.010313;-0.202355;,
  0.940987;0.017269;-0.338002;,
  -0.979259;0.010313;-0.202351;,
  -0.707119;-0.707094;-0.001048;,
  -0.687808;-0.698016;0.199235;,
  -0.658188;-0.675272;0.332859;,
  0.697196;0.686778;0.205559;,
  0.707118;0.707095;0.001049;,
  0.687807;0.698016;-0.199237;,
  0.658187;0.675272;-0.332864;,
  -0.697197;-0.686778;-0.205555;,
  -0.000001;-0.999999;-0.001483;,
  -0.000001;-0.980208;0.197970;,
  -0.000000;-0.943706;0.330785;,
  0.000000;0.978360;0.206910;,
  0.000000;0.999999;0.001485;,
  0.000000;0.980208;-0.197970;,
  0.000000;0.943706;-0.330786;,
  -0.000001;-0.978360;-0.206909;,
  0.707118;-0.707094;-0.001048;,
  0.687807;-0.698016;0.199236;,
  0.658188;-0.675273;0.332860;,
  -0.697196;0.686778;0.205556;,
  -0.707118;0.707095;0.001049;,
  -0.687807;0.698016;-0.199235;,
  -0.658188;0.675272;-0.332860;,
  0.697196;-0.686778;-0.205555;,
  -0.940990;-0.017268;0.337995;,
  -0.658189;-0.675274;0.332855;,
  0.940989;0.017268;-0.337995;,
  0.658187;0.675272;-0.332863;,
  0.000000;-0.943709;0.330777;,
  -0.000000;0.943705;-0.330789;,
  0.658188;-0.675273;0.332859;,
  -0.658187;0.675272;-0.332863;,
  0.940987;-0.017268;0.338002;,
  -0.940989;0.017268;-0.337995;,
  0.672798;0.655343;0.343319;,
  -0.672800;-0.655344;-0.343314;,
  0.000000;0.938391;0.345574;,
  -0.000000;-0.938392;-0.345574;,
  -0.672799;0.655344;0.343315;,
  0.672799;-0.655345;-0.343314;,
  1.000000;-0.000000;0.000000;,
  0.979259;-0.010313;0.202350;,
  0.940990;-0.017269;0.337993;,
  -0.979259;-0.010313;0.202352;,
  -1.000000;-0.000000;0.000000;,
  -0.979258;0.010313;-0.202353;,
  -0.940988;0.017268;-0.337999;,
  0.979259;0.010313;-0.202352;,
  0.707118;0.707095;0.001049;,
  0.697196;0.686778;0.205556;,
  0.672799;0.655344;0.343315;,
  -0.687808;-0.698016;0.199233;,
  -0.707119;-0.707094;-0.001049;,
  -0.697196;-0.686778;-0.205556;,
  -0.672798;-0.655344;-0.343317;,
  0.687806;0.698017;-0.199237;,
  0.000000;0.999999;0.001484;,
  0.000000;0.978360;0.206911;,
  0.000000;0.938390;0.345577;,
  -0.000000;-0.980209;0.197965;,
  -0.000000;-0.999999;-0.001484;,
  -0.000000;-0.978360;-0.206909;,
  -0.000000;-0.938392;-0.345574;,
  -0.000000;0.980208;-0.197971;,
  -0.707118;0.707094;0.001049;,
  -0.697196;0.686778;0.205557;,
  -0.672798;0.655343;0.343318;,
  0.687807;-0.698017;0.199234;,
  0.707118;-0.707095;-0.001049;,
  0.697196;-0.686778;-0.205557;,
  0.672798;-0.655344;-0.343316;,
  -0.687807;0.698016;-0.199237;,
  -1.000000;-0.000000;0.000000;,
  -0.979258;-0.010313;0.202352;,
  -0.940988;-0.017269;0.337998;,
  0.979258;-0.010312;0.202353;,
  1.000000;0.000001;0.000000;,
  0.979258;0.010314;-0.202353;,
  0.940989;0.017269;-0.337997;,
  -0.979258;0.010313;-0.202352;,
  -0.707118;-0.707094;-0.001049;,
  -0.687807;-0.698016;0.199235;,
  -0.658187;-0.675273;0.332861;,
  0.697196;0.686778;0.205557;,
  0.707118;0.707095;0.001049;,
  0.687807;0.698016;-0.199236;,
  0.658188;0.675273;-0.332860;,
  -0.697196;-0.686778;-0.205556;,
  -0.000001;-0.999999;-0.001483;,
  -0.000000;-0.980208;0.197968;,
  0.000000;-0.943707;0.330783;,
  0.000000;0.978360;0.206909;,
  0.000000;0.999999;0.001485;,
  0.000000;0.980208;-0.197970;,
  0.000000;0.943706;-0.330786;,
  -0.000000;-0.978360;-0.206909;,
  0.707118;-0.707095;-0.001048;,
  0.687807;-0.698017;0.199233;,
  0.658188;-0.675274;0.332857;,
  -0.697196;0.686778;0.205556;,
  -0.707118;0.707094;0.001049;,
  -0.687807;0.698016;-0.199236;,
  -0.658187;0.675272;-0.332861;,
  0.697196;-0.686779;-0.205555;,
  -0.940989;-0.017268;0.337996;,
  -0.658188;-0.675274;0.332856;,
  0.940989;0.017268;-0.337997;,
  0.658186;0.675272;-0.332864;,
  -0.000000;-0.943709;0.330778;,
  -0.000000;0.943705;-0.330789;,
  0.658188;-0.675274;0.332857;,
  -0.658186;0.675272;-0.332865;,
  0.940989;-0.017267;0.337996;,
  -0.940988;0.017268;-0.337999;,
  0.672799;0.655345;0.343314;,
  -0.672799;-0.655344;-0.343316;,
  0.000000;0.938392;0.345572;,
  0.000000;-0.938392;-0.345574;,
  -0.672799;0.655345;0.343314;,
  0.672799;-0.655345;-0.343314;;
  280;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;10,4,4,10;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;11,8,8,11;,
  4;8,1,1,8;,
  3;0,12,13;,
  3;0,14,12;,
  3;0,15,14;,
  3;0,16,15;,
  3;0,17,16;,
  3;0,17,17;,
  3;0,17,17;,
  3;0,13,17;,
  3;9,18,19;,
  3;9,19,20;,
  3;9,20,20;,
  3;9,20,20;,
  3;9,20,20;,
  3;9,20,20;,
  3;9,20,21;,
  3;9,21,18;,
  3;22,24,23;,
  3;22,25,24;,
  3;22,26,25;,
  3;22,27,26;,
  3;22,28,27;,
  3;22,29,28;,
  3;22,30,29;,
  3;22,23,30;,
  4;23,24,32,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,23,31,38;,
  4;31,32,40,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,31,39,46;,
  4;39,40,48,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,39,47,54;,
  4;47,48,56,55;,
  4;48,49,57,56;,
  4;49,50,58,57;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,47,55,62;,
  4;55,56,64,63;,
  4;56,57,65,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,55,63,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,66,74,73;,
  4;66,67,75,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,63,71,78;,
  3;71,72,79;,
  3;72,73,79;,
  3;73,74,79;,
  3;74,75,79;,
  3;75,76,79;,
  3;76,77,79;,
  3;77,78,79;,
  3;78,71,79;,
  3;80,81,82;,
  3;80,82,83;,
  3;80,83,84;,
  3;80,84,85;,
  3;80,85,86;,
  3;80,86,87;,
  3;80,87,88;,
  3;80,88,81;,
  4;81,89,90,82;,
  4;82,90,91,83;,
  4;83,91,92,84;,
  4;84,92,93,85;,
  4;85,93,94,86;,
  4;86,94,95,87;,
  4;87,95,96,88;,
  4;88,96,89,81;,
  4;89,97,98,90;,
  4;90,98,99,91;,
  4;91,99,100,92;,
  4;92,100,101,93;,
  4;93,101,102,94;,
  4;94,102,103,95;,
  4;95,103,104,96;,
  4;96,104,97,89;,
  4;97,105,106,98;,
  4;98,106,107,99;,
  4;99,107,108,100;,
  4;100,108,109,101;,
  4;101,109,110,102;,
  4;102,110,111,103;,
  4;103,111,112,104;,
  4;104,112,105,97;,
  4;105,113,114,106;,
  4;106,114,115,107;,
  4;107,115,116,108;,
  4;108,116,117,109;,
  4;109,117,118,110;,
  4;110,118,119,111;,
  4;111,119,120,112;,
  4;112,120,113,105;,
  4;113,121,122,114;,
  4;114,122,123,115;,
  4;115,123,124,116;,
  4;116,124,125,117;,
  4;117,125,126,118;,
  4;118,126,127,119;,
  4;119,127,128,120;,
  4;120,128,121,113;,
  4;121,129,130,122;,
  4;122,130,131,123;,
  4;123,131,132,124;,
  4;124,132,133,125;,
  4;125,133,134,126;,
  4;126,134,135,127;,
  4;127,135,136,128;,
  4;128,136,129,121;,
  3;129,137,130;,
  3;130,137,131;,
  3;131,137,132;,
  3;132,137,133;,
  3;133,137,134;,
  3;134,137,135;,
  3;135,137,136;,
  3;136,137,129;,
  4;138,139,147,146;,
  4;139,140,148,147;,
  4;202,141,149,203;,
  4;141,142,150,149;,
  4;142,143,151,150;,
  4;143,144,152,151;,
  4;204,145,153,205;,
  4;145,138,146,153;,
  4;146,147,155,154;,
  4;147,148,156,155;,
  4;203,149,157,206;,
  4;149,150,158,157;,
  4;150,151,159,158;,
  4;151,152,160,159;,
  4;205,153,161,207;,
  4;153,146,154,161;,
  4;154,155,163,162;,
  4;155,156,164,163;,
  4;206,157,165,208;,
  4;157,158,166,165;,
  4;158,159,167,166;,
  4;159,160,168,167;,
  4;207,161,169,209;,
  4;161,154,162,169;,
  4;162,163,171,170;,
  4;163,164,172,171;,
  4;208,165,173,210;,
  4;165,166,174,173;,
  4;166,167,175,174;,
  4;167,168,176,175;,
  4;209,169,177,211;,
  4;169,162,170,177;,
  4;170,171,179,178;,
  4;171,172,180,179;,
  4;210,173,181,212;,
  4;173,174,182,181;,
  4;174,175,183,182;,
  4;175,176,184,183;,
  4;211,177,185,213;,
  4;177,170,178,185;,
  4;178,179,187,186;,
  4;179,180,188,187;,
  4;212,181,189,214;,
  4;181,182,190,189;,
  4;182,183,191,190;,
  4;183,184,192,191;,
  4;213,185,193,215;,
  4;185,178,186,193;,
  4;186,187,195,194;,
  4;187,188,196,195;,
  4;214,189,197,216;,
  4;189,190,198,197;,
  4;190,191,199,198;,
  4;191,192,200,199;,
  4;215,193,201,217;,
  4;193,186,194,201;,
  4;194,195,139,138;,
  4;195,196,140,139;,
  4;216,197,141,202;,
  4;197,198,142,141;,
  4;198,199,143,142;,
  4;199,200,144,143;,
  4;217,201,145,204;,
  4;201,194,138,145;,
  4;218,219,227,226;,
  4;219,220,228,227;,
  4;282,221,229,283;,
  4;221,222,230,229;,
  4;222,223,231,230;,
  4;223,224,232,231;,
  4;284,225,233,285;,
  4;225,218,226,233;,
  4;226,227,235,234;,
  4;227,228,236,235;,
  4;283,229,237,286;,
  4;229,230,238,237;,
  4;230,231,239,238;,
  4;231,232,240,239;,
  4;285,233,241,287;,
  4;233,226,234,241;,
  4;234,235,243,242;,
  4;235,236,244,243;,
  4;286,237,245,288;,
  4;237,238,246,245;,
  4;238,239,247,246;,
  4;239,240,248,247;,
  4;287,241,249,289;,
  4;241,234,242,249;,
  4;242,243,251,250;,
  4;243,244,252,251;,
  4;288,245,253,290;,
  4;245,246,254,253;,
  4;246,247,255,254;,
  4;247,248,256,255;,
  4;289,249,257,291;,
  4;249,242,250,257;,
  4;250,251,259,258;,
  4;251,252,260,259;,
  4;290,253,261,292;,
  4;253,254,262,261;,
  4;254,255,263,262;,
  4;255,256,264,263;,
  4;291,257,265,293;,
  4;257,250,258,265;,
  4;258,259,267,266;,
  4;259,260,268,267;,
  4;292,261,269,294;,
  4;261,262,270,269;,
  4;262,263,271,270;,
  4;263,264,272,271;,
  4;293,265,273,295;,
  4;265,258,266,273;,
  4;266,267,275,274;,
  4;267,268,276,275;,
  4;294,269,277,296;,
  4;269,270,278,277;,
  4;270,271,279,278;,
  4;271,272,280,279;,
  4;295,273,281,297;,
  4;273,266,274,281;,
  4;274,275,219,218;,
  4;275,276,220,219;,
  4;296,277,221,282;,
  4;277,278,222,221;,
  4;278,279,223,222;,
  4;279,280,224,223;,
  4;297,281,225,284;,
  4;281,274,218,225;;
 }
}
