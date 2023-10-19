#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aTextureCoords;
layout(location = 2) in vec3 aNormal;

struct H2ETransformation {
  vec3 position;
  vec4 rotation;
  vec3 scale;
};


vec3 qtransform(vec4 q, vec3 v) {
  vec3 t = 2.0 * cross(q.yzw, v);
  return v + q.x * t + cross(q.yzw, t);
}

vec3 transform(H2ETransformation t, vec3 v) {
  // v = t.scale * v;
  vec3 u = t.position;
  // float gammaI = sqrt(1.0 - dot(t.position.xz, t.position.xz));
  // float gamma = 1.0 / gammaI;
  // v.xz = 1.0 / (1.0 + dot(t.position.xz, v.xz)) * (t.position.xz + (gammaI) * v.xz + (gamma / (1.0 + gamma)) * dot(t.position.xz, v.xz) * t.position.xz);
  // v.y += t.position.y;
  // return qtransform(t.rotation, v);
  v.xz = ((1.0 + 2.0 * dot(u.xz, v.xz) + dot(v.xz, v.xz)) * u.xz + (1.0 - dot(u.xz, u.xz)) * v.xz) /
    (1.0 + 2.0 * dot(u.xz, v.xz) + dot(u.xz, u.xz) * dot(v.xz, v.xz));
  v.y += t.position.y;
  return qtransform(t.rotation, v);
}

uniform H2ETransformation uModel;
uniform H2ETransformation uView;
uniform mat4 uProjection;

out vec3 normal;
out vec3 position;
out vec3 textureCoords;

void main() {
  textureCoords = aTextureCoords;
  normal = aNormal;
  
  position = aPosition.xyz;
  position *= uModel.scale;
  // position.xz /= 1.0 + sqrt(1.0 - dot(position.xz, position.xz));
  position /= 1.0 + sqrt(1.0 - dot(position, position));
  position = transform(uModel, position);
  vec3 pos = transform(uView, position);
  // float w  = 2.0 /(1.0 + dot(pos.xz, pos.xz));
  // gl_Position = uProjection * vec4(pos, w);
  vec4 projected = uProjection * vec4(pos, 1.0);
  // projected.xz *= (1.0 + dot(pos.xz, pos.xz)) / 2.0;
  projected *= (1.0 + dot(pos, pos)) / 2.0;
  gl_Position = projected;
}