// WARNING: Set the build definition correctly before running
#define PIR_BUILD
#define DEBUG true

#ifdef GAS_BUILD
    #include "nodes/node_gas.h"
#endif
#ifdef PIR_BUILD
    #include "nodes/node_pir.h"
#endif
#ifdef PLANT_BUILD
    #include "nodes/node_plant.h"
#endif
#ifdef LIVROOM_BUILD
    #include "nodes/node_livroom.h"
#endif

#ifdef TEST_BUILD
    #include "nodes/node_test.h"
#endif