#include "CG_Animation.h"

namespace CG
{

	/************************************************* KeyFrame **********************************************/

	KeyFrame::KeyFrame(Object* object_prt, TSpace space,
		mat4& prev_transform_matrix, mat4& next_transform_matrix) :
		object_prt(object_prt),
		space(space),
		prev_transform_matrix(prev_transform_matrix),
		next_transform_matrix(next_transform_matrix)
	{}
	
	/************************************************* AnimationRecord **********************************************/

	KeyFramesQueue AnimationRecord::getKeyFramesQueueCopy() const
	{
		return this->key_frames_queue;
	}

	Object* AnimationRecord::getRecordingObject() const
	{
		return recording_object;
	}

	TransformationsBuffer AnimationRecord::getInitialModelTransformHistory() const
	{
		return initial_model_transform_history;
	}

	TransformationsBuffer AnimationRecord::getInitialWorldTransformHistory() const
	{
		return initial_world_transform_history;
	}

	void AnimationRecord::initializeRecord(Object* object_ptr)
	{
		clearHistory();

		this->recording_object = object_ptr;
		fillHistoryBuffers(recording_object);

		initial_model_transform_history = model_transform_history;
		initial_world_transform_history = world_transform_history;
	}

	void AnimationRecord::clearHistory()
	{
		model_transform_history.clear();
		world_transform_history.clear();
	}

	void AnimationRecord::fillHistoryBuffers(Object* parent_object_ptr)
	{
		if (parent_object_ptr == nullptr)
			return;

		// remember parent_object transforms
		model_transform_history[parent_object_ptr] = parent_object_ptr->mTransform;
		world_transform_history[parent_object_ptr] = parent_object_ptr->wTransform;

		// remember child_object transforms
		for (auto& child_object : parent_object_ptr->children)
		{
			// recursively fill child objects history
			fillHistoryBuffers(&child_object);
		}
	}

	void AnimationRecord::captureFrame(Object* object, TSpace space)
	{
		if (object == nullptr)
			return;

		mat4 prev_mat, next_mat;
		if (space == OBJECT)
		{
			prev_mat = model_transform_history[object];
			next_mat = object->mTransform;
		}
		else if (space == VIEW)
		{
			prev_mat = world_transform_history[object];
			next_mat = object->wTransform;
		}

		if (prev_mat != next_mat)
		{
			// push the key_frame if the Transform matrix changes
			key_frames_queue.push(KeyFrame(object, space, prev_mat, next_mat));
			fillHistoryBuffers(recording_object);
		}
	}


	void AnimationRecord::pushObjectChanges(Object* object)
	{
		mat4 prev_mat, next_mat;
		// track changes in object model transform
		prev_mat = model_transform_history[object];
		next_mat = object->mTransform;
		if (prev_mat != next_mat)
		{
			key_frames_queue.push(KeyFrame(object, OBJECT, prev_mat, next_mat));
		}

		// track changes in object world transform
		prev_mat = world_transform_history[object];
		next_mat = object->wTransform;
		if (prev_mat != next_mat)
		{
			key_frames_queue.push(KeyFrame(object, VIEW, prev_mat, next_mat));
		}

		for (auto& child_object : object->children)
		{
			// recursively track changes in child objects
			pushObjectChanges(&child_object);
		}
	}


	void AnimationRecord::pushAllChanges()
	{
		pushObjectChanges(recording_object);
		fillHistoryBuffers(recording_object);
	}

	/************************************************* AnimationPlayer **********************************************/

	const double AnimationPlayer::FORWARD_FIRST_END = 1;
	const double AnimationPlayer::FORWARD_LAST_END = 0;
	const double AnimationPlayer::FORWARD_STEP = -1;

	AnimationPlayer::AnimationPlayer(AnimationRecord& record, double step, bool is_rewind):
		key_frames_queue(record.getKeyFramesQueueCopy()),
		key_frames_num(key_frames_queue.size()),
		first_end(is_rewind ? FORWARD_LAST_END : FORWARD_FIRST_END),
		last_end(is_rewind ? FORWARD_FIRST_END : FORWARD_LAST_END),
		step(is_rewind ? -FORWARD_STEP * step : FORWARD_STEP * step),
		total_frames_num(key_frames_num * (ceil((last_end - first_end) / step) + 1)),
		progress(first_end),
		current_frame_index(0)
	{
		restoreHistory(record.getRecordingObject(),
			record.getInitialModelTransformHistory(),
			record.getInitialWorldTransformHistory());
	}


	void AnimationPlayer::restoreHistory(Object* parent_object_ptr,
		TransformationsBuffer& model_transform_history, TransformationsBuffer& world_transform_history)
	{
		if (parent_object_ptr == nullptr)
			return;

		// restore child_object transforms
		for (auto& child_object : parent_object_ptr->children)
		{
			restoreHistory(&child_object, model_transform_history, world_transform_history);
		}

		// restore parent_object transforms
		parent_object_ptr->setMTransform(model_transform_history[parent_object_ptr]);
		parent_object_ptr->setWTransform(world_transform_history[parent_object_ptr]);
	}


	bool AnimationPlayer::nextKeyFrame()
	{
		if (key_frames_queue.empty())
			return false;

		current_key_frame = key_frames_queue.front();
		key_frames_queue.pop();

		return true;
	}


	bool AnimationPlayer::nextFrame()
	{
		if (progress == first_end)
		{
			// fresh start: next key-frame
			if (!nextKeyFrame())
				return false;
		}

		// calc the interpolated matrix according to progress
		mat4 interpolated_mat = mat4::InterpolatedMatrix(
			current_key_frame.prev_transform_matrix,
			current_key_frame.next_transform_matrix,
			progress);

		// set the model\ world transform according to space
		if (current_key_frame.space == OBJECT)
		{
			current_key_frame.object_prt->setMTransform(interpolated_mat);
		}
		else if (current_key_frame.space == VIEW)
		{
			current_key_frame.object_prt->setWTransform(interpolated_mat);
		}

		current_frame_index++;

		// update progress
		if (progress == last_end)
		{
			// next frame start fresh
			progress = first_end;
			return true;
		}

		progress += step;
		// if last step exceeds the last end:
		if (step > 0 && progress >= last_end) progress = last_end;
		else if (step < 0 && progress <= last_end) progress = last_end;

		return true;
	}

}