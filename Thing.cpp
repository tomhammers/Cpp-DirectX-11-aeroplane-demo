#include "DXUT.h"
#include "Thing.h"

Thing::Thing()
{
	init();
}

Thing::~Thing()
{
}

void Thing::init() {
	x = y = z = speed = 0.0f;
	rx = ry = rz = 0.0f;
	sx = sy = sz = 1.0f;
	frameTime = 0.001;
	XMVECTOR initialDir = XMVectorSet(0, 0, -1, 0);
	XMVECTOR viewerCam = XMVectorSet(0, 0, -1, 0);
}
// getter and setter for speed
HRESULT Thing::getSpeed()
{
	return this->speed;
	return S_OK;
}
HRESULT Thing::setSpeed(float speed)
{
	this->speed = speed;
	return S_OK;
}
HRESULT Thing::setThrottleSpeed(float speed)
{
	this->speed *= 52.0;
	return S_OK;
}
// getters and setters for position
HRESULT Thing::setPositionX(float x){
	this->x = x;
	return S_OK;
}
HRESULT Thing::setPositionY(float y){
	this->y = y;
	return S_OK;
}
HRESULT Thing::setPositionZ(float z){
	this->z = z;
	return S_OK;
}
float Thing::getPositionX(){
	return this->x;
}
float Thing::getPositionY(){
	return this->y;
}
float Thing::getPositionZ(){
	return this->z;
}
// method to move something (change position)
HRESULT Thing::moveThing(float x, float y, float z) {
	this->x += x;
	this->y += y;
	this->z += z;
	return S_OK;
}

// getters and setters for rotation
HRESULT Thing::setRotationX(float rx){
	this->rx = rx;
	return S_OK;
}
HRESULT Thing::setRotationY(float ry){
	this->ry = ry;
	return S_OK;
}
HRESULT Thing::setRotationZ(float rz){
	this->rz = rz;
	return S_OK;
}
float Thing::getRotationX(){
	return this->rx;
}
float Thing::getRotationY(){
	return this->ry;
}
float Thing::getRotationZ(){
	return this->rz;
}

// methods to rotate thing in certain axis
HRESULT Thing::rotateThingX(float rx){
	this->rx = rx;
	return S_OK;
}
HRESULT Thing::rotateThingY(float ry){
	this->ry += ry;
	return S_OK;
}
HRESULT Thing::rotateThingZ(float rz){
	this->rz = rz;
	return S_OK;
}

// getters and setters for scale
HRESULT Thing::setScaleX(float sx){
	this->sx = sx;
	return S_OK;
}
HRESULT Thing::setScaleY(float sy){
	this->sy = sy;
	return S_OK;
}
HRESULT Thing::setScaleZ(float sz){
	this->sz = sz;
	return S_OK;
}

float Thing::getScaleX(){
	return this->sx;
}
float Thing::getScaleY(){
	return this->sy;
}
float Thing::getScaleZ(){
	return this->sz;
}
// getter and setter for frametime
void Thing::setFrameTime(float frameTime){
	this->frameTime = frameTime;
}
float Thing::getFrameTime(){
	return this->frameTime;
}

