#include "PM_Interface.h"
#include "PM.hpp"

namespace PM {
static PathManager pathMng;
Path_Manager_Cycle_Status pathManStatus;


bool PathManagerInterfaceExecute(void) {

    do {
            pathMng.execute();
            pathManStatus = pathMng.getStatus();

            if (pathManStatus == FAILURE_MODE)
            {
                // Something is quite wrong, need to switch over to safety
                return false;
            }

    } while(pathManStatus != COMPLETED_CYCLE);
    return true;
}
}
