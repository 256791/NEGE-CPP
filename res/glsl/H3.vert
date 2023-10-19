#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTextureCoords;

struct Gyrovector3D {
  vec3 position;
  vec4 gyration;
  vec3 scale;
};

float norm2(vec3 v) {
  return dot(v, v);
}

// vec3 qtransform(vec4 q, vec3 v) {
//   vec3 t = 2.0 * cross(q.xyz, v);
//   return v + q.w * t + cross(q.xyz, t);
// }

vec3 qtransform(vec4 q, vec3 v) {
  vec3 t = 2.0 * cross(q.yzw, v);
  return v + q.x * t + cross(q.yzw, t);
}

vec4 qInv(vec4 q){
  return vec4(q.x,-q.y,-q.z,-q.w);
}

// vec3 gvtransfrom(Gyrovector3D a, vec3 b) {
//   // b = a.scale * b;
//   vec3 c = -cross(a.position, b);
//   float d = 1.0 + dot(a.position, b);
//   vec3 t = a.position + b;
//   return qtransform(a.gyration, (t * d + cross(c, t)) / (d * d + dot(c, c)));
// }

vec3 mobiusAdd(vec3 u, vec3 v) {
  return ((1.0 + 2.0 * dot(u, v) + dot(v, v)) * u + (1.0 - dot(u, u)) * v) /
    (1.0 + 2.0 * dot(u, v) + dot(u, u) * dot(v, v));
}

vec3 gvtransfrom(Gyrovector3D a, vec3 v) {
  return qtransform(a.gyration, mobiusAdd(a.position, v));
}

// vec3 gvtransfrom(Gyrovector3D u, vec3 v) {
//   v = u.scale *v;
//   float gammaI = sqrt(1.0 - norm2(u.position));
//   float gamma = 1.0 / gammaI;
//   v = 1.0 / (1.0 + dot(u.position, v)) * (u.position + (gammaI) * v + (gamma / (1.0 + gamma)) * dot(u.position, v) * u.position);
//   return qtransform(u.gyration, v);
// }

uniform Gyrovector3D uModel;
uniform Gyrovector3D uView;
uniform mat4 uProjection;

out vec3 normal;
out vec3 position;
out vec3 textureCoords;

void main() {
  textureCoords = aTextureCoords;

  normal = aNormal;
  normal = normal * uModel.scale;
  normal = qtransform(uModel.gyration, aNormal);

  position = aPosition.xyz;
  position = position * uModel.scale;
  position /= 1.0 + sqrt(1.0 - dot(position, position));


  position = qtransform(uModel.gyration, position);
  position = mobiusAdd(uModel.position, position);
  // position = qtransform(qInv(uModel.gyration), position);
  vec3 pos = position;
  position /= ((1.0 + dot(position, position)) / 2.0);
  
  pos = mobiusAdd(uView.position, pos);
  pos = qtransform(uView.gyration, pos);
  // pos = gvtransfrom(uView, pos);
  float w = 2.0 / (1.0 + dot(pos, pos));
  gl_Position = uProjection * vec4(pos, w);
}

// void main() {
//   textureCoords = aTextureCoords;

//   // normal = aNormal / (1.0 + sqrt(1.0 - dot(aNormal, aNormal)));
//   // normal = gvtransfrom(uModel, normal);
//   // normal /= ((1.0 + dot(normal, normal)) / 2.0);
//   // normal = qtransform(uModel.gyration,-aNormal);
//   normal = aNormal;
//   normal = normal * uModel.scale;
//   // normal = qtransform(qInv(uModel.gyration), normal);
//   normal = qtransform(uModel.gyration, aNormal);

//   position = aPosition.xyz;
//   position = position * uModel.scale;
//   position /= 1.0 + sqrt(1.0 - dot(position, position));
//   position = mobiusAdd(uModel.position, position);
//   vec3 pos = position;
//   // position = qtransform(qInv(uModel.gyration), position);
//   position = qtransform(uModel.gyration, position);
//   position /= ((1.0 + dot(position, position)) / 2.0);

//   pos = qtransform(uModel.gyration, pos);
//   pos = gvtransfrom(uView, pos);
//   float w = 2.0 / (1.0 + dot(pos, pos));
//   gl_Position = uProjection * vec4(pos, w);
// }