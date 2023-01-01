#ifndef CG_ANIMATION
#define CG_ANIMATION

#include <queue>
#include <map>
#include "CG_Matrix.h"
#include "CG_Object.h"

namespace CG
{
	class KeyFrame;
	typedef map<Object*, mat4> TransformationsBuffer;
	typedef queue<KeyFrame> KeyFramesQueue;

	class KeyFrame
	{
	public:
		Object* object_prt;
		TSpace space;
		mat4 prev_transform_matrix;
		mat4 next_transform_matrix;


		KeyFrame() = default;
		KeyFrame(Object* object_prt, TSpace space, 
			mat4& prev_transform_matrix, mat4& next_transform_matrix);

		~KeyFrame() = default;
	};


	class AnimationRecord
	{
	private:
		Object* recording_object;
		TransformationsBuffer model_transform_history;
		TransformationsBuffer world_transform_history;
		KeyFramesQueue key_frames_queue;

	public:
		AnimationRecord() = default;
		~AnimationRecord() = default;

		void initializeRecord(Object* object_ptr);
		void clearHistory();
		void fillHistoryBuffers(Object* parent_object_ptr);
		void captureFrame(Object* object, TSpace space);
	};
}

#endif