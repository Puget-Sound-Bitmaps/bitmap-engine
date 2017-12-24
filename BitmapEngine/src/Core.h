#ifndef CORE_H
#define CORE_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include "active/word/ActiveWord.h"
#include "active/run/ActiveRun.h"

#include "block-seg/BlockSeg.h"

#include "bbc/BBCUtil.h"
#include "bbc/BBCQuery.h"
#include "bbc/BBCCompressor.h"

#include "config/Vars.h"
#include "config/Control.h"

#include "clock/Clock.h"

#include "query/Query.h"
#include "query/QueryData.h"
#include "query/QueryUtil.h"

#include "raw-bitmap-reader/RawBitmapReader.h"

#include "seg-util/SegUtil.h"

#include "val/VALCompressor.h"
#include "val/VALQuery.h"

#include "wah/WAHCompressor.h"
#include "wah/WAHQuery.h"

#include "writer/Writer.h"

#endif /* CORE_H */
