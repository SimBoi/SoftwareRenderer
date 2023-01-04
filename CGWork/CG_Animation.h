#ifndef CG_ANIMATION
#define CG_ANIMATION

#include <vector>
#include <map>
#include "CG_Draw.h"
#include "CG_Matrix.h"
#include "CG_Object.h"

namespace CG
{
	typedef long FramesNum;
	typedef enum _RecordingStatus { EMPTY, INPROGRESS, STOPPED, PLAYING, PAUSED } RecordingStatus;

	class KeyFrame;
	typedef map<Object*, mat4> TransformationsBuffer;
	typedef vector<KeyFrame> KeyFramesQueue;

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
		TransformationsBuffer initial_model_transform_history;
		TransformationsBuffer initial_world_transform_history;
		TransformationsBuffer model_transform_history;
		TransformationsBuffer world_transform_history;
		KeyFramesQueue key_frames_queue;

		void initializeRecord(Object* object_ptr);

	public:
		AnimationRecord() = default;
		AnimationRecord(Object* recording_object);

		~AnimationRecord() = default;

		void clearHistory();
		void fillHistoryBuffers(Object* parent_object_ptr);
		void pushAllChanges();
		void pushObjectChanges(Object* object);
		void captureFrame(Object* object, TSpace space);

		Object* getRecordingObject() const;
		TransformationsBuffer getInitialModelTransformHistory() const;
		TransformationsBuffer getInitialWorldTransformHistory() const;
		TransformationsBuffer getLatestModelTransformHistory() const;
		TransformationsBuffer getLatestWorldTransformHistory() const;
		KeyFramesQueue getKeyFramesQueueCopy() const;
		FramesNum getKeyFramesNumber() const;
	};


	class AnimationPlayer
	{
	private:
		static const double FORWARD_FIRST_END;
		static const double FORWARD_LAST_END;
		static const double FORWARD_STEP;

		const double first_end, last_end;
		const double step;
		double progress;

		const FramesNum total_frames_num;
		FramesNum current_frame_index;
		KeyFrame current_key_frame;

		KeyFramesQueue key_frames_queue;

		void restoreHistory(Object* parent_object_ptr, 
			TransformationsBuffer& model_transform_history, TransformationsBuffer& world_transform_history);

		static FramesNum calcTotalFramesNum(AnimationRecord& record, double step);

	public:
		bool is_rewind;
		const double speed;
		const RenderMode playing_render_mode;
		
		AnimationPlayer() = default;
		AnimationPlayer(AnimationRecord& record, double step, RenderMode render_mode, 
			double speed = 0, bool is_rewind = false);
		~AnimationPlayer() = default;

		void updateProgress();

		mat4 getTransformMatrix() const;
		bool setTransformMatrix(mat4& new_mat);
		bool nextKeyFrame();
		bool nextFrame();

		FramesNum getTotalFramesNum() const;
		FramesNum getCurrentFrameIndex() const;
		double getProgressPercentage() const;
	};
}

#endif