#ifndef _DYNAMIC_DISK_H
#define _DYNAMIC_DISK_H


#include "Disk.h"

class DynamicDisk : public Disk
{

protected:
    rg_Point2D  m_VelocityVector;


    //For DVD
    double      m_Offset;
    double      m_MostRecentLocationUpdateTime;
    int         m_GroupId;
    bool        m_ThisIsContainer;
    bool        m_ThisIsActive;
    bool        m_ThisIsCreatedInDVD;

public:

    DynamicDisk();
    DynamicDisk(const int& ID, 
                const rg_Circle2D& circle, 
                const rg_Point2D& velocityVector, 
                const double& mostRecentLocationUpdateTime = 0.0,
                const double& offset = 0.0);
    DynamicDisk(const DynamicDisk& dynamicDisk);
    
    virtual ~DynamicDisk();
    virtual void copy_from(const DynamicDisk& dynamicDisk);

    double             getOffset()          const;
    double             getVelocityVectorX() const;
    double             getVelocityVectorY() const;
    const rg_Point2D&  getVelocityVector()  const;
    const rg_Circle2D& getCircle()          const;
    double             getMostRecentLoacationUpdateTime() const;
    int                getGroupId()                       const;

    void               setOffset(const double& offset);
    void               setVelocityVector(const double& vecX, const double& vecY);
    void               setVelocityVector(const rg_Point2D& velocityVector);
    void               setMostRecentLocationUpdateTime(const double& locationUpdateTime);
    void               setGroupId(const int& diskGroupId);

    void               set_this_disk_is_container(const bool& thisDiskIsContainer);
    void               set_this_is_active(const bool& thisIsActive);
    void               set_this_is_created_in_DVD(const bool& thisIsCreatedInDVD);

    bool               this_disk_is_container() const;
    bool               this_is_active()         const;
    bool               this_is_created_in_DVD() const;

    DynamicDisk& operator =(const DynamicDisk& dynamicDisk);
    bool        operator<(const DynamicDisk& dynamicDisk) const;
    bool        operator>(const DynamicDisk& dynamicDisk) const;
};


#endif

