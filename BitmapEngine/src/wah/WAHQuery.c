#include "../Core.h"

int op_wah(
    word_32*, word_32*, int, word_32*, int,
    word_32 (*)(word_32, int, word_32*, int*),
    word_32 (*)(word_32*, int*, word_32),
    word_32 (*)(word_32, word_32));

/**
 * OR two bit vectors together. If using this for WAH, ensure that
 * col[0] = col[1] = 0.
 * @param ret: array of results
 * @param col0: first column to query on
 * @param sz0: size of first column
 * @param col1, sz1 similar.
 */
int OR_WAH(word_32 *ret, word_32 *col0, int sz0, word_32 *col1, int sz1)
{
    return op_wah(ret, col0, sz0, col1, sz1, &fillORfillWAH,
        &fillORlitWAH, &litORlitWAH);
}

/**
 * AND two bit vectors together. If using this for WAH, ensure that
 * col[0] = col[1] = 0.
 * @param ret: array of results
 * @param col0: first column to query on
 * @param sz0: size of first column
 * @param col1, sz1 similar.
 */
int AND_WAH(word_32 *ret, word_32 *col0, int sz0, word_32 *col1, int sz1)
{
    return op_wah(ret, col0, sz0, col1, sz1, &fillANDfillWAH,
        &fillANDlitWAH, &litANDlitWAH);
}

int op_wah(
    word_32 *ret, word_32 *col0, int sz0, word_32 *col1, int sz1,
    word_32 (*fill_op_fill_wah)(word_32, int, word_32*, int*),
    word_32 (*fill_op_lit_wah)(word_32*, int*, word_32),
    word_32 (*lit_op_lit_wah)(word_32, word_32))
{
    int c0;       /* word number we're scanning from col0 */
    int c1;
    int d;        /* spot we're saving into the result */
    c0 = c1 = 1;
    d = 0;        /* start saving into the first spot */

    word_32 w0 = col0[c0++];
    word_32 w1 = col1[c1++];
    int t0 = getType(w0, WORD_LENGTH);
    int t1 = getType(w1, WORD_LENGTH);

    while (c0 <= sz0 && c1 <= sz1) {
        word_32 toAdd; /* result of operating on w0, w1 */
        /* two literals */
        if (t0 < ZERO_RUN && t1 < ZERO_RUN) {
            toAdd = lit_op_lit_wah(w0, w1);
            w0 = col0[c0++];
            t0 = getType(w0, WORD_LENGTH);
            w1 = col1[c1++];
            t1 = getType(w1, WORD_LENGTH);
        }
        /* one literal, one fill */
        else if (t0 < ZERO_RUN || t1 < ZERO_RUN) {
            if (t0 < ZERO_RUN) { /* w0 is the literal */
                toAdd = fill_op_lit_wah(&w1, &t1, w0);
                w0 = col0[c0++];
                t0 = getType(w0,WORD_LENGTH);
            }
            else { /* w1 is the literal */
                toAdd = fill_op_lit_wah(&w0, &t0, w1);
                w1 = col1[c1++];
                t1 = getType(w1,WORD_LENGTH);
            }
        }
        /* Two fills */
        else {
            if ((w0 << 2) < (w1 << 2)) { /* w0 is smaller */
                toAdd = fill_op_fill_wah(w0, t0, &w1, &t1);
                w0 = col0[c0++]; /* update smaller fill */
                t0 = getType(w0, WORD_LENGTH);
            }
            else if ((w0 << 2) > (w1 << 2)) { /* w1 is smaller */
                toAdd = fill_op_fill_wah(w1, t1, &w0, &t0);
                w1 = col1[c1++];
                t1 = getType(w1, WORD_LENGTH);
            }
            /* Special case: equal size fills, which we treat as literals */
            else {
                toAdd = lit_op_lit_wah(w0, w1);
                w0 = col0[c0++];
                t0 = getType(w0, WORD_LENGTH);
                w1 = col1[c1++];
                t1 = getType(w1, WORD_LENGTH);
            }
        }
        /* If this is the first word, append it to the end of the result
         * column. */
        if (d >= 1) {
            if(appendWAH(ret, toAdd, &d)) {
                puts("AppendWAH failure");
                return -1;
            }
        }
        /* This is the first word, which is one of two things:
         * number of words, if being used for VAL
         * 0, if being used for WAH. */
        else {
            ret[++d] = toAdd;
        }
    }
    /* the number of words we have written. Note that clients using this
     * result for future WAH queries should add 1 to get the total
     * number of vectors being operated on. */
    return d + 1;
}

unsigned int veclen(word_32 *v)
{
    return sizeof(v) / sizeof(word_32);
}

/**
 * Adds the wordToAdd to the end (d = last added position) of the addTo sequence
 * wordToAdd will be consolidated into position if possible and if not (or the
 * leftover) will go into ret[d + 1].
 */
int appendWAH(word_32 *addTo, word_32 wordToAdd, int *d)
{
    int prevT = getType(addTo[*d], WORD_LENGTH); /* type of the last added */
    if (prevT == LITERAL) { /* there's no way to consolidate */
        if (*d > veclen(addTo) - 1) {
            puts("Error: cannot append words.");
            return 1;
        }
        addTo[++(*d)] = wordToAdd;
        return 0;
    }
    int addT = getType(wordToAdd, WORD_LENGTH); // type of the one we're adding

    if (prevT == ZERO_RUN) {
        /* both zero runs so we might be able to consolidate if there's
         * enough room */
        if (addT == ZERO_RUN) {
            /* helps to check the stopping condition (as long as there are
             * still fills left) */
            word_32 minCheck = getZeroFill(BASE_LEN) - 2;
            word_32 maxCheck = getMaxZeroFill(BASE_LEN);
            while (wordToAdd > minCheck && addTo[(*d)] < maxCheck) {
                addTo[(*d)]++;
                wordToAdd--;
            }
            /* we've successfully added everything */
            if (wordToAdd == minCheck) {
                return 0;
            }
            /* ran out of space */
            else {
                /* there was exactly one left so switch to literal
                 * before adding */
                if (wordToAdd == minCheck + 1) {
                    wordToAdd = 0;
                }
                addTo[++(*d)] = wordToAdd;
                return 0;
            }
        }
        /* we can probably just add this one lit to the previous run */
        else if (addT == ZERO_LIT) {
            /* not maxed out yet, so just add it */
            if (addTo[(*d)] < getMaxZeroFill(BASE_LEN)) {
                addTo[(*d)]++;
            }
            /* maxed out so can't consolidate */
            else {
                addTo[++(*d)] = wordToAdd; /* save into the next spot */
                return 0;
            }
        }
        else { /* can't consolidate */
            addTo[++(*d)] = wordToAdd;
            return 0;
        }
    }
    else if (prevT == ZERO_LIT) {
        if (addT == ZERO_LIT) { /* consolidate two literals into one fill */
            addTo[(*d)] = getZeroFill(BASE_LEN);
            return 0;
        }
        else if (addT == ZERO_RUN) {
            /* not maxed out yet, so add it */
            if (wordToAdd < getMaxZeroFill(BASE_LEN)) {
                addTo[(*d)] = wordToAdd + 1;
                return 0;
            }
            else { /* maxed out */
                addTo[(*d) + 1] = addTo[(*d)];
                addTo[(*d)++] = wordToAdd;
                return 0;
            }
        }
        else { /* can't consolidate */
            addTo[++(*d)] = wordToAdd;
            return 0;
        }
    }
    else if (prevT == ONE_RUN) {
        /* both run of ones so might be able to consolidate */
        if (addT == ONE_RUN) {
            /* helps to check the stopping condition
             * (as long as there are still fills left) */
            word_32 minCheck = getOneFill(BASE_LEN) - 2;
            word_32 maxCheck = getMaxOneFill(BASE_LEN);
            while (wordToAdd > minCheck && addTo[(*d)] < maxCheck) {
                addTo[(*d)]++;
                wordToAdd--;
            }
            if (wordToAdd == minCheck) {
                return 0; /* successfully added everything to previous */
            }
            else { /* ran out of space */
                /* there was exactly one left so switch to literal
                 * before adding */
                if(wordToAdd == minCheck + 1) {
                    wordToAdd = getZeroFill(BASE_LEN) - 3;
                }
                addTo[++(*d)] = wordToAdd;
                return 0;
            }
        }
        else if (addT == ONE_LIT) {
            /* previous isn't maxed out so just add it */
            if (addTo[(*d)] < getMaxOneFill(BASE_LEN)) {
                addTo[(*d)]++;
                return 0;
            }
            else { /* maxed out so can't consolidate */
                addTo[++(*d)] = wordToAdd;
                return 0;
            }
        }
        else {
            addTo[++(*d)] = wordToAdd;
            return 0;
        }
    }
    else { /* prev is lit of ones */
        if(addT == ONE_LIT) {
            addTo[(*d)] = getOneFill(BASE_LEN);
            return 0;
        }
        else if (addT == ONE_RUN) {
            if (wordToAdd < getMaxOneFill(BASE_LEN)) { /* not maxed out */
                addTo[(*d)] = wordToAdd+1;
            }
            else { /* maxed out so can't consolidate */
                addTo[(*d) + 1] = addTo[(*d)];
                addTo[(*d)++] = wordToAdd;
                return 0;
            }
        }
        else { /* can't consolidate */
            addTo[++(*d)] = wordToAdd;
            return 0;
        }
    }
    return 0;
}

word_32
fill_op_fill_wah(word_32, int, word_32 *, int *, word_32 (*)(word_32));

word_32
fillANDfillWAH(word_32 smallFill, int smallT, word_32 *bigFill, int *bigT)
{
    return fill_op_fill_wah(smallFill, smallT, bigFill, bigT, &getZeroFill);
}

word_32
fillORfillWAH(word_32 smallFill, int smallT, word_32 *bigFill, int *bigT)
{
    return fill_op_fill_wah(smallFill, smallT, bigFill, bigT, &getOneFill);
}

/**
 * Performs the op on two fill words. Updates the larger fill, returns the
 * result word
 */
word_32
fill_op_fill_wah(word_32 smallFill, int smallT, word_32 *bigFill,
    int *bigT, word_32 (*ret_fill)(word_32))
{
    word_32 ret;
    /* what we are going to subtract from the larger word */
    word_32 sub = ((smallFill << 2) >> 2);
    /* if both 0/1 runs, must return small run of 0/1s */
    if ((smallT == ONE_RUN && *bigT == ONE_RUN)
        || (smallT == ZERO_RUN && *bigT == ZERO_RUN)) {
        ret = smallFill;
    }
    else {
        /* build run of 1s of length sub (number of runs in smallFill) */
        ret = ret_fill(BASE_LEN) - 2 + sub;
    }
    *bigFill -= sub;
    /* check to see if we subtracted too much */
    if (((*bigFill << 2) >> 2) == 1) { /* need to change to literal */
        if (*bigT == ZERO_RUN) {
            *bigFill = 0; /* literal run of zeros */
            *bigT = ZERO_LIT;
        }
        else { /* update the larger fill for the future */
            *bigFill = getZeroFill(BASE_LEN) - 3;
            *bigT = ONE_LIT;
        }
    }

    return ret;
}

word_32 fill_op_lit_wah(word_32*, int*, word_32, int);

word_32 fillORlitWAH(word_32 *fill, int *fillT, word_32 lit)
{
    return fill_op_lit_wah(fill, fillT, lit, 1);
}

word_32 fillANDlitWAH(word_32 *fill, int *fillT, word_32 lit)
{
    return fill_op_lit_wah(fill, fillT, lit, 0);
}

/**
 * Performs an op on a fill and a literal word
 * Updates the fill for the future
 * Returns the resulting word
 */
word_32 fill_op_lit_wah(word_32 *fill, int *fillT, word_32 lit, int is_or)
{
    word_32 ret; /* the word to be returned */
    if (is_or) {
        /* whatever we or with a run of 1s will be 1 so just return
         * lit of ones (later) */
        if (*fillT == ONE_RUN) {
            /* literal run of 1s */
            ret = getZeroFill(BASE_LEN) - 3;
        }
        /* otherwise we have a run of 1s so the result will be whatever
         * the literal is */
        else {
            ret = lit;
        }
    }
    else {
        /* anding with 0 just returns 0 anyway. */
        if (*fillT == ZERO_RUN) {
            ret = 0;
        }
        else {
            ret = lit;
        }
    }

    /* do we need to turn result into literal run? */
    if (((*fill << 2) >> 2) == 2) {
        if (*fillT == ZERO_RUN) {
            *fill = 0;
            *fillT = ZERO_LIT;
        }
        else {
            *fill = getZeroFill(BASE_LEN) - 3;
            *fillT = ONE_LIT;
        }
    }
    else {
        *fill -= 1;
    }

    return ret;
}

/**
 * Performs an OR between 2 literals and returns the resulting word
 */
word_32 litORlitWAH(word_32 lit1, word_32 lit2)
{
    return lit1 | lit2; /* just or them together */
}

/**
 * Performs an AND between 2 literals and returns the resulting word
 */
word_32 litANDlitWAH(word_32 lit1, word_32 lit2)
{
    return lit1 & lit2; /* just and them together */
}
