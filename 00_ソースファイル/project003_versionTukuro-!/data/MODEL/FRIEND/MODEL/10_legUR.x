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
 121;
 -3.34299;-14.67788;-2.77396;,
 -3.22374;-14.70014;-1.11867;,
 -4.19160;-14.01533;-1.15797;,
 -4.37465;-13.84607;-3.27852;,
 -4.48654;-9.64736;-1.59016;,
 -5.40564;-3.23753;-1.87355;,
 -4.95103;-2.43285;-4.30720;,
 -4.54044;-9.16524;-3.46857;,
 2.78089;-13.67586;4.00722;,
 2.33618;-14.85358;2.94885;,
 3.70682;-14.85736;1.73250;,
 4.56313;-13.54430;2.35466;,
 -5.12148;-3.12172;1.28719;,
 -4.16405;-9.69696;0.55150;,
 -3.18097;-9.35322;2.63991;,
 -4.31000;-2.73784;3.41684;,
 4.60523;-14.88830;-0.91850;,
 5.68253;-13.43132;-0.85211;,
 3.08857;-13.25832;-6.13879;,
 2.50905;-14.80559;-4.93643;,
 -0.16079;-14.75421;-5.37827;,
 -0.32977;-13.43125;-6.58070;,
 -2.48829;-14.70025;-4.39267;,
 -3.33543;-13.65787;-5.29802;,
 0.23096;-14.73643;3.15185;,
 0.16086;-13.84621;4.42169;,
 4.13497;-14.86468;-3.27390;,
 5.11035;-13.30273;-3.97834;,
 -3.30130;-8.34880;-5.55383;,
 -3.43899;-0.51580;-6.53415;,
 -0.29316;-0.07301;-7.48149;,
 -0.25326;-6.87645;-6.78041;,
 -2.96671;-14.71299;0.25532;,
 -3.86841;-14.10209;0.60819;,
 -2.03711;-14.73755;1.60048;,
 -2.69707;-14.06842;2.36527;,
 3.07821;-6.01303;-6.92041;,
 4.90630;-5.72953;-4.52379;,
 5.39987;-5.75263;-1.17852;,
 2.88656;-6.76994;4.69189;,
 -0.15337;-8.11664;4.95880;,
 4.77214;-6.01709;2.24546;,
 3.05449;-0.01683;-7.17510;,
 4.94260;-0.10224;-4.76822;,
 5.55922;-0.20594;-1.46230;,
 4.82321;-0.21724;2.32599;,
 2.95624;-0.16156;4.81026;,
 -1.44095;-13.94974;3.55232;,
 -1.07593;-14.72940;2.53505;,
 -1.81772;-8.73642;4.08485;,
 -2.61824;-2.03598;5.11123;,
 -0.35788;-1.28863;5.57833;,
 -0.17521;0.98437;-5.46917;,
 -2.29649;0.68499;-4.83731;,
 0.60749;1.91006;-1.20477;,
 -3.10565;0.61302;-3.57742;,
 0.60749;1.91006;-1.20477;,
 -3.52986;0.64376;-1.84359;,
 0.60749;1.91006;-1.20477;,
 -3.40347;0.88021;0.39991;,
 0.60749;1.91006;-1.20477;,
 -2.79779;1.01794;1.69337;,
 0.60749;1.91006;-1.20477;,
 -1.47679;1.09952;3.06376;,
 0.60749;1.91006;-1.20477;,
 -0.12782;1.07971;3.30673;,
 0.60749;1.91006;-1.20477;,
 2.01085;0.92267;2.85274;,
 0.60749;1.91006;-1.20477;,
 3.27128;0.88980;1.15780;,
 0.60749;1.91006;-1.20477;,
 3.63622;0.98686;-1.40151;,
 0.60749;1.91006;-1.20477;,
 3.33063;0.97240;-3.60297;,
 0.60749;1.91006;-1.20477;,
 2.08029;1.02238;-5.24202;,
 0.60749;1.91006;-1.20477;,
 -0.17521;0.98437;-5.46917;,
 0.60749;1.91006;-1.20477;,
 -4.66595;-0.62933;-4.65227;,
 2.95624;-0.16156;4.81026;,
 -0.25620;0.12362;5.40627;,
 -5.29228;-0.45251;-2.12460;,
 -5.10786;-0.23975;1.20655;,
 -4.19787;-0.02772;3.14762;,
 -2.26030;0.09443;5.12796;,
 -0.35788;-1.28863;5.57833;,
 -0.15337;-8.11664;4.95880;,
 2.88656;-6.76994;4.69189;,
 2.95624;-0.16156;4.81026;,
 -3.43899;-0.51580;-6.53415;,
 -3.30130;-8.34880;-5.55383;,
 -4.54044;-9.16524;-3.46857;,
 -4.95103;-2.43285;-4.30720;,
 0.65821;-14.27731;-1.38901;,
 -2.48829;-14.70025;-4.39267;,
 -0.16079;-14.75421;-5.37827;,
 0.65821;-14.27731;-1.38901;,
 -3.34299;-14.67788;-2.77396;,
 0.65821;-14.27731;-1.38901;,
 -3.22374;-14.70014;-1.11867;,
 0.65821;-14.27731;-1.38901;,
 -2.96671;-14.71299;0.25532;,
 0.65821;-14.27731;-1.38901;,
 -2.03711;-14.73755;1.60048;,
 0.65821;-14.27731;-1.38901;,
 -1.07593;-14.72940;2.53505;,
 0.65821;-14.27731;-1.38901;,
 0.23096;-14.73643;3.15185;,
 0.65821;-14.27731;-1.38901;,
 2.33618;-14.85358;2.94885;,
 0.65821;-14.27731;-1.38901;,
 3.70682;-14.85736;1.73250;,
 0.65821;-14.27731;-1.38901;,
 4.60523;-14.88830;-0.91850;,
 0.65821;-14.27731;-1.38901;,
 4.13497;-14.86468;-3.27390;,
 0.65821;-14.27731;-1.38901;,
 2.50905;-14.80559;-4.93643;,
 0.65821;-14.27731;-1.38901;,
 -0.16079;-14.75421;-5.37827;;
 
 85;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,5,4,13;,
 4;14,15,12,13;,
 4;10,16,17,11;,
 4;18,19,20,21;,
 4;20,22,23,21;,
 4;24,9,8,25;,
 4;17,16,26,27;,
 4;28,29,30,31;,
 4;26,19,18,27;,
 4;2,1,32,33;,
 4;32,34,35,33;,
 4;23,22,0,3;,
 4;28,23,3,7;,
 4;33,35,14,13;,
 4;4,2,33,13;,
 4;27,18,36,37;,
 4;38,17,27,37;,
 4;25,8,39,40;,
 4;21,23,28,31;,
 4;36,18,21,31;,
 4;11,17,38,41;,
 4;39,8,11,41;,
 4;3,2,4,7;,
 4;42,36,31,30;,
 4;43,37,36,42;,
 4;44,38,37,43;,
 4;45,41,38,44;,
 4;46,39,41,45;,
 4;47,35,34,48;,
 4;24,25,47,48;,
 4;49,14,35,47;,
 4;25,40,49,47;,
 4;50,15,14,49;,
 4;40,51,50,49;,
 3;52,53,54;,
 3;53,55,56;,
 3;55,57,58;,
 3;57,59,60;,
 3;59,61,62;,
 3;61,63,64;,
 3;63,65,66;,
 3;65,67,68;,
 3;67,69,70;,
 3;69,71,72;,
 3;71,73,74;,
 3;73,75,76;,
 3;75,77,78;,
 3;29,6,79;,
 3;80,81,51;,
 4;82,79,6,5;,
 4;12,83,82,5;,
 4;15,84,83,12;,
 4;50,85,84,15;,
 4;51,81,85,50;,
 4;52,30,29,53;,
 4;67,46,45,69;,
 4;69,45,44,71;,
 4;71,44,43,73;,
 4;73,43,42,75;,
 4;75,42,30,77;,
 4;79,55,53,29;,
 4;80,67,65,81;,
 4;82,57,55,79;,
 4;83,59,57,82;,
 4;84,61,59,83;,
 4;85,63,61,84;,
 4;81,65,63,85;,
 4;86,87,88,89;,
 4;90,91,92,93;,
 3;94,95,96;,
 3;97,98,95;,
 3;99,100,98;,
 3;101,102,100;,
 3;103,104,102;,
 3;105,106,104;,
 3;107,108,106;,
 3;109,110,108;,
 3;111,112,110;,
 3;113,114,112;,
 3;115,116,114;,
 3;117,118,116;,
 3;119,120,118;;
 
 MeshMaterialList {
  1;
  85;
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
  0;;
  Material {
   0.360800;0.204000;0.128800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  82;
  -0.953099;-0.078131;-0.292400;,
  -0.945513;-0.120203;0.302582;,
  0.912343;-0.019813;-0.408947;,
  -0.107540;-0.104324;0.988712;,
  -0.147031;-0.073166;-0.986422;,
  0.897031;-0.017732;0.441612;,
  -0.965638;-0.066046;0.251359;,
  -0.024411;-0.002552;0.999699;,
  -0.928141;-0.032355;-0.370820;,
  -0.994244;-0.091407;0.055882;,
  -0.623351;-0.075144;-0.778323;,
  0.999571;-0.001590;0.029242;,
  -0.788155;-0.148056;0.597404;,
  0.452709;-0.059188;-0.889692;,
  0.497306;-0.056086;0.865760;,
  -0.998549;-0.042431;-0.033158;,
  -0.821130;-0.059091;0.567674;,
  0.697352;-0.613860;-0.369968;,
  -0.219372;-0.972700;0.075695;,
  0.150161;-0.936706;-0.316280;,
  -0.257179;-0.963738;-0.071189;,
  0.304738;-0.932212;0.195234;,
  -0.044040;-0.941946;-0.332865;,
  -0.253005;-0.967406;0.010679;,
  -0.191455;-0.956137;-0.221689;,
  0.798543;-0.600270;0.044778;,
  -0.662343;-0.513405;0.545635;,
  0.419832;-0.385184;-0.821812;,
  0.418799;-0.432095;0.798688;,
  0.811789;-0.345527;0.470754;,
  -0.125125;-0.395011;-0.910115;,
  -0.131713;-0.497671;0.857307;,
  0.846739;-0.332210;-0.415535;,
  -0.808243;-0.508762;0.296486;,
  -0.855706;-0.473109;-0.209608;,
  -0.872750;-0.480865;0.084115;,
  -0.583832;-0.442075;-0.680963;,
  0.950067;-0.308135;0.049255;,
  0.128041;-0.951575;0.279481;,
  -0.051827;-0.962762;0.265337;,
  -0.178281;-0.971068;0.158877;,
  -0.135118;-0.967210;0.215056;,
  -0.485290;-0.509977;0.710223;,
  -0.531589;-0.144727;0.834546;,
  -0.483222;-0.039367;0.874613;,
  -0.023208;0.999677;-0.010343;,
  -0.130945;-0.093421;-0.986978;,
  0.457762;-0.054967;-0.887374;,
  0.916073;-0.026972;-0.400103;,
  0.876526;0.481120;0.015044;,
  0.885695;0.312069;0.343741;,
  -0.301477;0.919629;-0.251781;,
  -0.103469;0.938815;-0.328512;,
  -0.390070;0.907048;-0.158461;,
  -0.408198;0.907942;-0.094956;,
  -0.395035;0.918645;0.006283;,
  -0.321335;0.936665;0.139290;,
  -0.154991;0.955405;0.251353;,
  0.047005;0.951118;0.305229;,
  0.229413;0.936008;0.266945;,
  0.368410;0.916256;0.157319;,
  0.412112;0.910991;0.016069;,
  0.357879;0.924168;-0.133553;,
  0.163442;0.946860;-0.277025;,
  -0.797843;0.500836;-0.335575;,
  -0.683196;-0.065993;-0.727247;,
  0.007838;0.093609;0.995578;,
  0.414236;0.012388;0.910085;,
  -0.858970;0.502542;-0.098095;,
  -0.846647;0.507831;0.159050;,
  -0.717561;0.545330;0.433268;,
  -0.392068;0.589006;0.706650;,
  -0.007205;-0.999913;-0.011025;,
  -0.112743;0.882656;-0.456298;,
  -0.372206;0.857521;-0.355134;,
  0.283470;0.890765;0.355223;,
  0.405426;0.868587;0.284932;,
  0.459021;0.871040;-0.174895;,
  0.214719;0.901072;-0.376781;,
  0.045304;0.907274;0.418092;,
  -0.151945;-0.988350;-0.008815;,
  -0.138611;-0.988954;0.052504;;
  85;
  4;20,23,35,34;,
  4;9,15,8,0;,
  4;28,38,21,29;,
  4;6,15,9,1;,
  4;12,16,6,1;,
  4;21,25,37,29;,
  4;27,19,22,30;,
  4;22,24,36,30;,
  4;39,38,28,31;,
  4;37,25,17,32;,
  4;10,65,46,4;,
  4;17,19,27,32;,
  4;35,23,18,33;,
  4;18,40,26,33;,
  4;36,24,20,34;,
  4;10,36,34,0;,
  4;33,26,12,1;,
  4;9,35,33,1;,
  4;32,27,13,2;,
  4;11,37,32,2;,
  4;31,28,14,3;,
  4;30,36,10,4;,
  4;13,27,30,4;,
  4;29,37,11,5;,
  4;14,28,29,5;,
  4;34,35,9,0;,
  4;47,13,4,46;,
  4;48,2,13,47;,
  4;49,11,2,48;,
  4;50,5,11,49;,
  4;67,14,5,50;,
  4;42,26,40,41;,
  4;39,31,42,41;,
  4;43,12,26,42;,
  4;31,3,43,42;,
  4;44,16,12,43;,
  4;3,7,44,43;,
  3;52,51,45;,
  3;51,53,45;,
  3;53,54,45;,
  3;54,55,45;,
  3;55,56,45;,
  3;56,57,45;,
  3;57,58,45;,
  3;58,59,45;,
  3;59,60,45;,
  3;60,61,45;,
  3;61,62,45;,
  3;62,63,45;,
  3;63,52,45;,
  3;65,8,64;,
  3;67,66,7;,
  4;68,64,8,15;,
  4;6,69,68,15;,
  4;16,70,69,6;,
  4;44,71,70,16;,
  4;7,66,71,44;,
  4;52,73,74,51;,
  4;59,75,76,60;,
  4;60,50,49,61;,
  4;61,49,77,62;,
  4;62,77,78,63;,
  4;63,78,73,52;,
  4;64,53,51,74;,
  4;75,59,58,79;,
  4;68,54,53,64;,
  4;69,55,54,68;,
  4;70,56,55,69;,
  4;71,57,56,70;,
  4;79,58,57,71;,
  4;7,3,14,67;,
  4;65,10,0,8;,
  3;72,24,22;,
  3;72,20,24;,
  3;72,23,20;,
  3;72,18,23;,
  3;72,40,18;,
  3;72,41,40;,
  3;72,39,41;,
  3;72,38,39;,
  3;72,21,38;,
  3;72,80,21;,
  3;72,81,80;,
  3;72,19,81;,
  3;72,22,19;;
 }
 MeshTextureCoords {
  121;
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.125000;,
  0.076920;0.125000;,
  0.038460;0.000000;,
  0.153850;0.125000;,
  0.115380;0.000000;,
  0.230770;0.125000;,
  0.192310;0.000000;,
  0.307690;0.125000;,
  0.269230;0.000000;,
  0.384620;0.125000;,
  0.346150;0.000000;,
  0.461540;0.125000;,
  0.423080;0.000000;,
  0.538460;0.125000;,
  0.500000;0.000000;,
  0.615380;0.125000;,
  0.576920;0.000000;,
  0.692310;0.125000;,
  0.653850;0.000000;,
  0.769230;0.125000;,
  0.730770;0.000000;,
  0.846150;0.125000;,
  0.807690;0.000000;,
  0.923080;0.125000;,
  0.884620;0.000000;,
  1.000000;0.125000;,
  0.961540;0.000000;,
  0.153850;0.125000;,
  0.615380;0.125000;,
  0.538460;0.125000;,
  0.230770;0.125000;,
  0.307690;0.125000;,
  0.384620;0.125000;,
  0.461540;0.125000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  2.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  2.000000;0.000000;,
  0.038460;1.000000;,
  0.076920;0.500000;,
  0.000000;0.500000;,
  0.115380;1.000000;,
  0.153850;0.500000;,
  0.192310;1.000000;,
  0.230770;0.500000;,
  0.269230;1.000000;,
  0.307690;0.500000;,
  0.346150;1.000000;,
  0.384620;0.500000;,
  0.423080;1.000000;,
  0.461540;0.500000;,
  0.500000;1.000000;,
  0.538460;0.500000;,
  0.576920;1.000000;,
  0.615380;0.500000;,
  0.653850;1.000000;,
  0.692310;0.500000;,
  0.730770;1.000000;,
  0.769230;0.500000;,
  0.807690;1.000000;,
  0.846150;0.500000;,
  0.884620;1.000000;,
  0.923080;0.500000;,
  0.961540;1.000000;,
  1.000000;0.500000;;
 }
}
