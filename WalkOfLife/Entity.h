#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#endif

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#include <vector>

using namespace DirectX;
using namespace std;
//using namespace DirectX::TriangleTests;

class Entity{
protected:
	bool isActive; //ska vi rendera detta eller inte? jag lägger denna här ifall vi tex vill kunna stänga av ljus oxå
	bool isStatic;
	XMMATRIX rot;
	XMMATRIX scale;
	float xInterval, yInterval, xSpeed, ySpeed;
	XMFLOAT3 currIntervalPosition;



	

public:
	XMFLOAT3 playerStartIntervalPosition; //används som ett sorts index för moving objects, se PlayerObject TestDown()
	XMMATRIX pos;
	float xPos;
	float yPos;


	int indexT;
	//Slam Variables
	float slamWaitTime;
	float slamDeltaTime;
	float slamStopTime;
	float slamStartTime;
	bool slamHasStopped;
	float slamMomentum;
	//bool slamHasBegun;
	bool slamReturning;
	int slamDirection;
	float slamSpeedMultiplier;
	bool slamReset;
	float slamTimeOffset;
	float slamTimeInternal;
	int slamSlamRuns;
	int slamResetNums;
	float slamCurrentDirection;
	float slamCurrentSpeed;
	

	Entity(XMFLOAT3 pos, bool isActive, bool isStatic){
		this->isActive = isActive;
		this->isStatic = isStatic;
		this->xPos = pos.x;
		this->yPos = pos.y;

		this->currIntervalPosition = XMFLOAT3(0, 0, 0);
		this->playerStartIntervalPosition = XMFLOAT3(0, 0, 0);
		xInterval = 0;
		yInterval = 0;
		xSpeed = 0;
		ySpeed = 0;

		if (pos.x < 0.000001f && pos.x > -0.0000001f && pos.y < 0.000001f && pos.y > -0.0000001f){
			this->pos = XMMatrixIdentity();
			this->world = XMMatrixIdentity();
		}
		else
			Translate(pos.x, pos.y, pos.z);

		rot = XMMatrixIdentity(); //den rotationen och skalningen som den importeras in som kommer vara standard värdet
		scale = XMMatrixIdentity();


		//Slam Test Variable initializations
		slamWaitTime = 1.0f;
		slamDeltaTime = 0.0f;
		slamStopTime = 0.0f;
		slamStartTime = 0.0f;
		slamMomentum = 0.0f;
		slamReturning = false;
		slamHasStopped = false;
		slamDirection = (int)xSpeed;
		slamSpeedMultiplier = 1.0f;
		slamReset = false;
		slamTimeOffset = 0.0f;
		slamSlamRuns = 0;
		slamResetNums = -1;
		

	}

	Entity(){
		this->currIntervalPosition = XMFLOAT3(0, 0, 0);
	}
	~Entity(){

	}

	XMMATRIX world;

	void Translate(float x, float y, float z){
		pos = XMMatrixTranslation(this->xPos + x, this->yPos + y, z);
	}

	void TranslateExact(float x, float y, float z){
		//this->xPos = x;
		//this->yPos = y;
		pos = XMMatrixTranslation(x, y, z);
	}

	void Rotate(XMVECTOR rotAxis, float rotValue){
		rot = XMMatrixRotationAxis(rotAxis, rotValue);
	}

	void Scale(float x, float y, float z){
		scale = XMMatrixScaling(x, y, z);
	}

	void CalculateWorld(){ //denna ska kallas innan man skickar in den i GPUn
		world = scale * rot * pos; //stämmer detta?
	}

	void SetActive(bool a){
		isActive = a; //används ifall vi vill stänga av detta object, tex när man plockar upp den
	}

	bool GetActive(){ return isActive; }
	bool GetStatic(){ return isStatic; }

	float GetXInterval(void)
	{
		return xInterval;
	}
	
	float GetYInterval(void)
	{
		return yInterval;
	}

	void PatrolInterval(float time){
		//applya xSpeed och ySpeed på nått sätt
		this->currIntervalPosition.x = sinf(xSpeed*time) * xInterval;
		this->currIntervalPosition.y = sinf(ySpeed*time) * yInterval;
		this->currIntervalPosition.z = 0;
		this->Translate(currIntervalPosition.x, currIntervalPosition.y, currIntervalPosition.z);
	}

	void SlamaReseta(float time)
	{

		this->TranslateExact(0.0f, yPos, 0.0f);
		slamReset = true;
	}

	void SlamaJamma(float time, int runTimes)
	{
		//tempP.slamDirection = coinValue;
		//tempP.slamSpeedMultiplier = ySpeed;
		//tempP.slamWaitTime = xSpeed;
		//tempP.slamTimeOffset = xInterval;

		if (runTimes == 0)
		{
			slamSlamRuns = 0;
			this->TranslateExact(0.0f, 0.0f, 0.0f);
			this->currIntervalPosition.y =  0.0f;
			slamStartTime = 0.0f;
			slamTimeInternal = 0.0f;
			slamHasStopped = false;
			slamMomentum = 0.0f;
			slamResetNums = -1;
			slamDeltaTime = 0.0f;
			slamStopTime = 0.0f;
			slamStartTime = 0.0f;
			slamMomentum = 0.0f;
			slamReturning = false;
			slamReset = false;
			slamCurrentDirection = slamDirection;
			slamCurrentSpeed = slamSpeedMultiplier;

		}
		
		else if (runTimes == 1)
		{
			if (slamSlamRuns < 1)
			{
				slamSlamRuns = runTimes;
			}
			if (slamSlamRuns >= 1)
			{
				if (slamSlamRuns == 1)
				{
					slamStartTime = time;
					//slamTimeInternal = time;
				}
				else if (slamSlamRuns > 1)
				{

					if (slamReset)
					{
						slamStartTime = time;
						//slamTimeInternal = time + slamTimeOffset;
						slamReset = false;
					}

					slamTimeInternal = time - slamStartTime;

					if (!slamHasStopped)
					{
						if (slamResetNums < 1)
						{
							if (slamTimeInternal > slamTimeOffset)
							{
								slamMomentum = (slamTimeInternal)* slamCurrentDirection * slamCurrentSpeed;
								this->currIntervalPosition.y = currIntervalPosition.y + slamMomentum;
							}
						}
						else
						{
							slamMomentum = (slamTimeInternal)* slamCurrentDirection * slamCurrentSpeed;
							this->currIntervalPosition.y = currIntervalPosition.y + slamMomentum;
						}

					}

					if (!slamReturning)
					{

						if (slamCurrentDirection < 0 && currIntervalPosition.y > -yInterval || slamCurrentDirection > 0 && currIntervalPosition.y < yInterval)
						{
							this->Translate(currIntervalPosition.x, currIntervalPosition.y, currIntervalPosition.z);
							//this->yPos = currIntervalPosition.y;
						}
						else
						{
							if (!slamHasStopped)
							{
								slamStopTime = (slamTimeInternal);
								slamMomentum = 0.0f;
								slamHasStopped = true;
							}

							slamDeltaTime = (slamTimeInternal)-(slamStopTime);

							if (slamDeltaTime >= slamWaitTime)
							{
								slamCurrentDirection = slamCurrentDirection * -1.0f;
								slamCurrentSpeed = 0.1f; //Decrease the speed on Slammer return cycle
								slamMomentum = 0.0f;
								slamStartTime = time;
								slamReturning = true;
								slamHasStopped = false;
								slamDeltaTime = 0.0f;
								slamReset = true;
								slamResetNums++;
							}

						}
					}
					else
					{

						if (slamCurrentDirection < 0 && currIntervalPosition.y > 0 || slamCurrentDirection > 0 && currIntervalPosition.y < 0)
						{
							this->Translate(currIntervalPosition.x, currIntervalPosition.y, currIntervalPosition.z);
							//this->yPos = currIntervalPosition.y;
						}
						else
						{
							if (!slamHasStopped)
							{
								slamStopTime = (slamTimeInternal);
								slamMomentum = 0.0f;
								slamHasStopped = true;
							}

							slamDeltaTime = (slamTimeInternal)-slamStopTime;
							if (slamDeltaTime >= slamWaitTime)
							{
								slamCurrentDirection = slamCurrentDirection * -1.0f;
								slamCurrentSpeed = 1.0f; //Set full speed for slam
								slamMomentum = 0.0f;
								slamStartTime = time;
								slamReturning = false;
								slamHasStopped = false;
								slamDeltaTime = 0.0f;
								slamReset = true;
								slamResetNums++;
							}

						}
					}

				}
				slamSlamRuns++;
			}

		}
	
		//Slam Direction is set by the Coin Value in the custom format Exporter.
		//Slam Object type is 6
		// slamSpeedMultiplier is set by ySpeed
		// slamWaitTime by timeValue
		// slamTimeOffset by xInterval
		
		
	}

	XMFLOAT3 GetCurrIntervalPos(){
		return currIntervalPosition;
	}

	XMFLOAT3 SubXMFLOAT3(XMFLOAT3 a, XMFLOAT3 b){
		return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	bool SameXMFLOAT3(XMFLOAT3 a, XMFLOAT3 b){
		if ((a.x - b.x < 0.00001f) && (a.x - b.x > -0.00001f) && (a.y - b.y < 0.00001f) && (a.y - b.y > -0.00001f) && (a.z - b.z < 0.00001f) && (a.z - b.z > -0.00001f))
			return true;
		else return false;
	}
};