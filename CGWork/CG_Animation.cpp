#include "CG_Animation.h"

namespace CG
{
	KeyFrame::KeyFrame(Object* object_prt, TSpace space,
		mat4& prev_transform_matrix, mat4& next_transform_matrix) :
		object_prt(object_prt),
		space(space),
		prev_transform_matrix(prev_transform_matrix),
		next_transform_matrix(next_transform_matrix)
	{}
	


	void AnimationRecord::initializeRecord(Object* object_ptr)
	{
		this->recording_object = object_ptr;

		clearHistory();
		fillHistoryBuffers(recording_object);
	}

	void AnimationRecord::clearHistory()
	{
		model_transform_history.clear();
		world_transform_history.clear();
	}

	void AnimationRecord::fillHistoryBuffers(Object* parent_object_ptr)
	{
		// remember parent_object transforms
		model_transform_history.insert({ parent_object_ptr, parent_object_ptr->mTransform });
		world_transform_history.insert({ parent_object_ptr, parent_object_ptr->wTransform });

		// remember child_object transforms
		for (auto& child_object : parent_object_ptr->children)
		{
			model_transform_history.insert({ &child_object, parent_object_ptr->mTransform });
			world_transform_history.insert({ &child_object, parent_object_ptr->wTransform });
		}
	}

	void AnimationRecord::captureFrame(Object* object, TSpace space)
	{
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

		key_frames_queue.push(KeyFrame(object, space, prev_mat, next_mat));
		fillHistoryBuffers(recording_object);
	}
}