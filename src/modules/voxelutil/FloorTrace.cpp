/**
 * @file
 */

#include "FloorTrace.h"
#include "core/Trace.h"
#include "core/Common.h"

namespace voxelutil {

FloorTraceResult findWalkableFloor(voxel::PagedVolume::Sampler *sampler, const glm::ivec3& position, int maxDistanceY) {
	core_trace_scoped(FindWalkableFloor);
	sampler->setPosition(position);
	if (!sampler->currentPositionValid()) {
		return FloorTraceResult();
	}

	const voxel::VoxelType type = sampler->voxel().getMaterial();
	if (voxel::isEnterable(type)) {
		const int maxDistance = core_min(maxDistanceY, position.y);
		for (int i = 0; i < maxDistance; ++i) {
			sampler->moveNegativeY();
			if (!sampler->currentPositionValid()) {
				break;
			}
			const voxel::VoxelType mat = sampler->voxel().getMaterial();
			if (!voxel::isEnterable(mat)) {
				return FloorTraceResult(sampler->position().y + 1, sampler->voxel());
			}
		}
		return FloorTraceResult();
	}

	const int maxDistance = core_min(maxDistanceY, voxel::MAX_HEIGHT - position.y);
	for (int i = 0; i < maxDistance; ++i) {
		sampler->movePositiveY();
		if (!sampler->currentPositionValid()) {
			break;
		}
		const voxel::VoxelType mat = sampler->voxel().getMaterial();
		if (voxel::isEnterable(mat)) {
			return FloorTraceResult(sampler->position().y, sampler->voxel());
		}
	}
	return FloorTraceResult();
}

FloorTraceResult findWalkableFloor(voxel::PagedVolume* volume, const glm::ivec3& position, int maxDistanceY) {
	voxel::PagedVolume::Sampler sampler(volume);
	return findWalkableFloor(&sampler, position, maxDistanceY);
}

}