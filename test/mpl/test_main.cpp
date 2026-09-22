// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/tdd/test/test.h"

#include "test_bool.h"
#include "test_int.h"
#include "test_integralconstant.h"
#include "test_list.h"
#include "test_nothing.h"
#include "test_always.h"
#include "test_isinstance.h"

#include "functional/test_bind.h"
#include "functional/test_call.h"
#include "functional/test_compose.h"
#include "functional/test_flow.h"
#include "functional/test_fork.h"
#include "functional/test_identity.h"

#include "sequence/test_at.h"
#include "sequence/test_islist.h"
#include "sequence/test_join.h"
#include "sequence/test_makesequence.h"
#include "sequence/test_popfront.h"
#include "sequence/test_pushback.h"
#include "sequence/test_pushfront.h"
#include "sequence/test_size.h"

#include "functions/bitwise/test_bitwiseand.h"
#include "functions/bitwise/test_bitwisecompliment.h"
#include "functions/bitwise/test_bitwiseor.h"
#include "functions/bitwise/test_bitwisexor.h"

#include "functions/comparison/test_equal.h"
#include "functions/comparison/test_greaterthan.h"
#include "functions/comparison/test_greaterthanorequal.h"
#include "functions/comparison/test_lessthan.h"
#include "functions/comparison/test_lessthanorequal.h"
#include "functions/comparison/test_notequal.h"

#include "functions/logical/test_logicaland.h"
#include "functions/logical/test_logicalnot.h"
#include "functions/logical/test_logicalor.h"

#include "functions/arithmetic/test_decrement.h"
#include "functions/arithmetic/test_divide.h"
#include "functions/arithmetic/test_increment.h"
#include "functions/arithmetic/test_invert.h"
#include "functions/arithmetic/test_max.h"
#include "functions/arithmetic/test_min.h"
#include "functions/arithmetic/test_minus.h"
#include "functions/arithmetic/test_modulo.h"
#include "functions/arithmetic/test_negate.h"
#include "functions/arithmetic/test_plus.h"
#include "functions/arithmetic/test_times.h"

#include "algorithm/test_all.h"
#include "algorithm/test_any.h"
#include "algorithm/test_combinations.h"
#include "algorithm/test_count_if.h"
#include "algorithm/test_extreme.h"
#include "algorithm/test_filter.h"
#include "algorithm/test_findif.h"
#include "algorithm/test_flatten.h"
#include "algorithm/test_foldleft.h"
#include "algorithm/test_foldright.h"
#include "algorithm/test_group.h"
#include "algorithm/test_lookup.h"
#include "algorithm/test_none.h"
#include "algorithm/test_partition.h"
#include "algorithm/test_product.h"
#include "algorithm/test_removeadjacent.h"
#include "algorithm/test_removeif.h"
#include "algorithm/test_reverse.h"
#include "algorithm/test_rotate.h"
#include "algorithm/test_sort.h"
#include "algorithm/test_splitif.h"
#include "algorithm/test_stablesort.h"
#include "algorithm/test_transform.h"
#include "algorithm/test_zipwith.h"

int main(int, const char**, char**)
{
    return 0;
}

