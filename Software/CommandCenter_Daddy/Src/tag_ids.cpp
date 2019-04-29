#include "tag_ids.hpp"

struct tag find_tag(uint8_t * uid) {
    uint8_t * uid_strt = uid;
    for (int ii=0; ii<num_tags; ii++) {
        uid = uid_strt;
        for (int jj=0; jj<7; jj++) {
            if (tags[ii].uid[jj] == *uid) {
                if (jj == 6) {
                    return tags[ii];
                }
            } else {
            	break;
            }
            uid++;
        }
    }
    return tags[0];
}

