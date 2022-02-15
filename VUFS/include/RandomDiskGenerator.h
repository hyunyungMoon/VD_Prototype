#ifndef _RANDOM_DISK_GENERATOR_
#define _RANDOM_DISK_GENERATOR_

#include "Disk.h"
#include "DynamicDisk.h"
#include "BucketForDisk.h"
#include <list>
#include <random>
using namespace std;

class RandomDiskGenerator
{
private:
    mt19937 m_RandomNumberGenerator;

public: 
    static const int DEFAULT_CONTAINER_ID;
    static const int NUM_INTERATION_PER_A_DISK_EMBED;

    //Function1. Disk generation
    bool generate_random_disks_in_circle_container(const int& numDisks,
                                                          const double& minRadius,
                                                          const double& maxRadius,
                                                          const double& ratioOfSumOfDisksAreaVsContainerArea,
                                                          const bool& diskCanBeIntersected,
                                                          list<Disk>& generatedDisks,
                                                          const rg_Point2D& containerCenterPt = rg_Point2D(0.0,0.0),
                                                          const bool& containCircleContainerInGeneratedDiskList = false,
                                                          const unsigned int& seedNumber = 0,
                                                          const int& numInterationPerADiskEmbed = NUM_INTERATION_PER_A_DISK_EMBED);

    bool generate_random_disks_in_circle_container(const int& numDisks,
                                                          const double& minRadius,
                                                          const double& maxRadius,
                                                          const bool& diskCanBeIntersected,
                                                          const rg_Circle2D& container,
                                                          list<Disk>& generatedDisks,
                                                          const bool& containCircleContainerInGeneratedDiskList = false,
                                                          const unsigned int& seedNumber = 0,
                                                          const int& numInterationPerADiskEmbed = NUM_INTERATION_PER_A_DISK_EMBED);

    pair<bool, Disk> generate_a_random_disk_in_circle_container(const int& id,
                                                           const double& minRadius,
                                                           const double& maxRadius,
                                                           const rg_Circle2D& container,
                                                           const unsigned int& seedNumber = 0,
                                                           const int& numInterationPerADiskEmbed = NUM_INTERATION_PER_A_DISK_EMBED);

     pair<bool, Disk> generate_a_random_disk_in_circle_container_not_intersectd_with_other_disks(
                                                           const int& id,
                                                           const double& minRadius,
                                                           const double& maxRadius,
                                                           const list<rg_Circle2D>& disks,
                                                           const rg_Circle2D& container,
                                                           const unsigned int& seedNumber = 0,
                                                           const int& numInterationPerADiskEmbed = NUM_INTERATION_PER_A_DISK_EMBED);


    bool generate_random_moving_disks_in_circle_container(const int& numDisks,
                                                                 const double& minRadius,
                                                                 const double& maxRadius,
                                                                 const double& minMagnitudeOfVelocityVector,
                                                                 const double& maxMagnitudeOfVelocityVector,
                                                                 const double& ratioOfSumOfDisksAreaVsContainerArea,
                                                                 const bool& diskCanBeIntersected,
                                                                 list<DynamicDisk>& generatedMovingDisks,
                                                                 const rg_Point2D& containerCenterPt = rg_Point2D(0.0,0.0),
                                                                 const bool& containCircleContainerInGeneratedDiskList = false,
                                                                 const unsigned int& seedNumber = 0,
                                                                 const int& numInterationPerADiskEmbed = NUM_INTERATION_PER_A_DISK_EMBED);

    bool generate_random_moving_disks_in_circle_container(const int& numDisks,
                                                                 const double& minRadius,
                                                                 const double& maxRadius,
                                                                 const double& minMagnitudeOfVelocityVector,
                                                                 const double& maxMagnitudeOfVelocityVector,
                                                                 const bool& diskCanBeIntersected,
                                                                 const rg_Circle2D& container,
                                                                 list<DynamicDisk>& generatedMovingDisks,
                                                                 const bool& containCircleContainerInGeneratedDiskList = false,
                                                                 const unsigned int& seedNumber = 0,
                                                                 const int& numInterationPerADiskEmbed = NUM_INTERATION_PER_A_DISK_EMBED);


    bool generate_random_disks_in_bounding_box(const int& numDisks,
                                                      const double& minRadius,
                                                      const double& maxRadius,
                                                      const double& ratioOfSumOfDisksAreaVsContainerArea,
                                                      const bool& diskCanBeIntersected,
                                                      list<Disk>& generatedDisks,
                                                      const unsigned int& seedNumber = 0,
                                                      const int& numInterationPerADiskEmbed = NUM_INTERATION_PER_A_DISK_EMBED);

    bool generate_random_moving_disks_in_bounding_box(const int& numDisks,
                                                             const double& minRadius,
                                                             const double& maxRadius,
                                                             const double& minMagnitudeOfVelocityVector,
                                                             const double& maxMagnitudeOfVelocityVector,
                                                             const double& ratioOfSumOfDisksAreaVsContainerArea,
                                                             const bool& diskCanBeIntersected,
                                                             list<DynamicDisk>& generatedMovingDisks,
                                                             const unsigned int& seedNumber = 0,
                                                             const int& numInterationPerADiskEmbed = NUM_INTERATION_PER_A_DISK_EMBED);

    //Function2. File writing

private:
    bool generate_random_disks_in_circle_container_with_bucket(const int& numDisks,
                                                                      const double& minRadius,
                                                                      const double& maxRadius,
                                                                      const rg_Circle2D& container,
                                                                      const bool& diskCanBeIntersected,
                                                                      BucketForDisk& bucketForDisk,
                                                                      list<Disk>& generatedDisks,
                                                                      const int& numInterationPerADiskEmbed = NUM_INTERATION_PER_A_DISK_EMBED);


    double compute_radius_of_bounding_circle(const int& numDisks,
                                                    const double& minRadius,
                                                    const double& maxRadius,
                                                    const double& ratioOfSumOfDisksAreaVsContainerArea);
    
    double compute_length_of_one_side_of_square_bounding_box(const int& numOfDisks, 
                                                                    const double& minRadius,
                                                                    const double& maxRadius,
                                                                    const double& ratioOfSumOfDisksAreaVsContainerArea);

    bool this_disk_is_in_circle_container(const Disk& disk, const rg_Circle2D& circleContainer);
    
    bool this_disk_is_in_bounding_box(const Disk& disk, const rg_Point2D& minPt, const rg_Point2D& maxPt);

    bool this_is_included_in_other_disks_vice_versa(const Disk& disk, const BucketForDisk& bucketForDisk);

    bool this_disk_is_intersected_with_other_disks(const Disk& disk, const BucketForDisk& bucketForDisk);

    bool these_two_disks_are_intersected(const rg_Circle2D& circle1, const rg_Circle2D& circle2, const double& res);

    void allocate_random_velocity_vector_to_disks(
        const double& minMagnitudeOfVelocityVector,const double& maxMagnitudeOfVelocityVector, 
        const list<Disk>& randomDisks, list<DynamicDisk>& randomMovingDisks);

    void translate_disks(list<Disk>& generatedDisks, const rg_Point2D& translationIncrement);
};



#endif // 
