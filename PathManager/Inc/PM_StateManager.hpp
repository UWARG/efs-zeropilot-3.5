/**
 *  Path Manager State Machine Header
 */

#ifndef ZPSW3_PM_STATE_MANAGER_HPP
#define ZPSW3_PM_STATE_MANAGER_HPP

#include "PM.hpp"

namespace PM {
class PathManager;

class PathManagerState
{
    public:
        virtual void enter(PathManager* pathMgr) = 0;
        virtual void execute(PathManager* pathMgr) = 0;
        virtual void exit(PathManager* pathMgr) = 0;

        bool operator==(const PathManagerState& rhs) const {return (this == &rhs);} 

        virtual ~PathManagerState() {}

};
}

#endif

