// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#include "SkeletonPose.hpp"
#include "Skeleton.hpp"

namespace Pomdog {

SkeletonPose SkeletonPose::CreateBindPose(Skeleton const& skeleton)
{
    SkeletonPose skeletonPose;
    skeletonPose.JointPoses.reserve(skeleton.JointCount());
    for (auto & joint: skeleton) {
        skeletonPose.JointPoses.push_back(joint.BindPose);
    }

    return std::move(skeletonPose);
}

} // namespace Pomdog
