//
//  FILE NAME:       easing.h
//
//  DESCRIPTION:     Class for holding easing function
//

#pragma once

typedef float(*easingFuncPtr)(float);

namespace NEasing
{
	// linear
	float linear(float t);

    // Quad functions
    float inQuad( float t );
    float outQuad( float t );
    float inOutQuad( float t );

    // Cube functions
    float inCubic( float t );
    float outCubic( float t );
    float inOutCubic( float t );
 
    // Quart functions
    float inQuart( float t );
    float outQuart( float t );
    float inOutQuart( float t );

    // Sine functions
    float inSine( float t );
    float outSine( float t );
    float inOutSine( float t );

    // Back functions
    float inBack( float t );
    float outBack( float t );
    float inOutBack( float t );

    // Circle functions
    float inCirc( float t );
    float outCirc( float t );
    float inOutCirc( float t );
 
    // Bounce functions
    float inBounce( float t );
    float outBounce( float t );
    float inOutBounce( float t );

    // Elastic functions 
    float inElastic( float t );
    float outElastic( float t );
    float inOutElastic( float t );
}

class CEasing
{
public:

	CEasing();
	~CEasing();

	// Init the script for use
	void init( float start, float end, float totalTime, easingFuncPtr easingFunc, bool resetCurrent = false );

	// Execute this class object
	void execute();

	// Set back to a pre-init state
	void clear(bool clearEasingFunc = false);

	// Get the current value
	float getValue();

	// Force the easing to finish
	void finish();

	// Finished access function
	bool isFinished();

	// Is this class initialized
	bool isInitialized();

private:

	float m_start;
	float m_end;
	float m_current;
	float m_time;
	float m_totalTime;
	float m_dif;
	bool m_finished;
	easingFuncPtr m_easingFunc;
};

