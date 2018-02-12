#include "../Core.h"

 int op_wah(
     word_32*, word_32*, int, word_32*, int,
     word_32 (*)(word_32, int, word_32*, int*),
     word_32 (*)(word_32*, int*, word_32),
     word_32 (*)(word_32, word_32)
 );

 int OR_WAH(word_32 *ret, word_32 *col0, int sz0, word_32 *col1, int sz1)
 {
     return op_wah(ret, col0, sz0, col1, sz1, &fillORfillWAH, &fillORlitWAH, &litORlitWAH);
 }

 int AND_WAH(word_32 *ret, word_32 *col0, int sz0, word_32 *col1, int sz1)
 {
     return op_wah(ret, col0, sz0, col1, sz1, &fillANDfillWAH, &fillANDlitWAH, &litANDlitWAH);
 }

 int op_wah(
     word_32 *ret,
     word_32 *col0,
     int sz0,
     word_32 *col1,
     int sz1,
     word_32 (*fill_op_fill_wah)(word_32, int, word_32*, int*),
     word_32 (*fill_op_lit_wah)(word_32*, int*, word_32),
     word_32 (*lit_op_lit_wah)(word_32, word_32)
 )
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
         word_32 toAdd;
         if (t0 < ZERO_RUN && t1 < ZERO_RUN) {
             toAdd = lit_op_lit_wah(w0, w1);
             w0 = col0[c0++];
             t0 = getType(w0, WORD_LENGTH);
             w1 = col1[c1++];
             t1 = getType(w1, WORD_LENGTH);
         }
         else if (t0 < ZERO_RUN || t1 < ZERO_RUN) {
             if (t0 < ZERO_RUN) {
                 toAdd = fill_op_lit_wah(&w1, &t1, w0);
                 w0 = col0[c0++];
                 t0 = getType(w0,WORD_LENGTH);
             }
             else {
                 toAdd = fill_op_lit_wah(&w0, &t0, w1);
                 w1 = col1[c1++];
                 t1 = getType(w1,WORD_LENGTH);
             }
         }
         else {
             if ((w0 << 2) < (w1 << 2)) {
                 toAdd = fill_op_fill_wah(w0, t0, &w1, &t1);
                 w0 = col0[c0++];
                 t0 = getType(w0, WORD_LENGTH);
             }
             else if ((w0 << 2) > (w1 << 2)) {
                 toAdd = fill_op_fill_wah(w1, t1, &w0, &t0);
                 w1 = col1[c1++];
                 t1 = getType(w1,WORD_LENGTH);
             }
             else {
                 toAdd = lit_op_lit_wah(w0, w1);
                 w0 = col0[c0++];
                 t0 = getType(w0, WORD_LENGTH);
                 w1 = col1[c1++];
                 t1 = getType(w1, WORD_LENGTH);
             }
         }
         if (d >= 1) {
             appendWAH(ret, toAdd, &d);
         }
         else {
             ret[++d] = toAdd;
         }
     }
     return d + 1;
 }

/*
 * Adds the wordToAdd to the end (d=last added position) of the addTo sequence
 * wordToAdd will be consolidated into position if possible and if not (or the leftover) will go into d+1
 */
void appendWAH(word_32 *addTo, word_32 wordToAdd, int *d)
{

    int prevT = getType(addTo[*d], WORD_LENGTH); // type of the last added

    if (prevT == LITERAL) { // there's no way to consolidate
        addTo[++(*d)] = wordToAdd;
        return;
    }
    int addT = getType(wordToAdd, WORD_LENGTH); // type of the one we're adding

    if (prevT == ZERO_RUN) {
        if (addT == ZERO_RUN) { // both zero runs so we might be able to consolidate if there's enough room
            word_32 minCheck = getZeroFill(BASE_LEN) - 2; // helps to check the stopping condition (as long as there are still fills left)
            word_32 maxCheck = getMaxZeroFill(BASE_LEN);
            while (wordToAdd > minCheck && addTo[(*d)] < maxCheck) {
                addTo[(*d)]++;
                wordToAdd--;
            }
            if (wordToAdd == minCheck) {
                return; // successfully added everything to previous
            }
            else { // stopped because ran out of space
                if (wordToAdd == minCheck + 1) { // there was exactly one left so switch to literal before adding
                    wordToAdd = 0;
                }
                addTo[++(*d)] = wordToAdd;
                return;
            }
        }
        else if (addT == ZERO_LIT) { // we can probably just add this one lit to the previous run
            if (addTo[(*d)] < getMaxZeroFill(BASE_LEN)) { // not maxed out yet, so just add it
                addTo[(*d)]++;
            }
            else { // maxed out so can't consolidate
                addTo[++(*d)] = wordToAdd; // save into the next spot
                return;
            }
        }
        else { // can't consolidate
            addTo[++(*d)] = wordToAdd;
            return;
        }
    }
    else if (prevT == ZERO_LIT) {
        if (addT == ZERO_LIT) { // consolidate two literals into one fill
            addTo[(*d)] = getZeroFill(BASE_LEN);
            return;
        }
        else if (addT == ZERO_RUN) {
            if (wordToAdd < getMaxZeroFill(BASE_LEN)) { // not maxed out yet, so add it
                addTo[(*d)] = wordToAdd + 1;
                return;
            }
            else { // maxed out
                addTo[(*d) + 1] = addTo[(*d)];
                addTo[(*d)++] = wordToAdd;
                return;
            }
        }
        else { // can't consolidate
            addTo[++(*d)] = wordToAdd;
            return;
        }
    }
    else if (prevT == ONE_RUN) {
        if (addT == ONE_RUN) { // both run of ones so might be able to consolidate
            word_32 minCheck = getOneFill(BASE_LEN) - 2; // helps to check the stopping condition (as long as there are still fills left)
            word_32 maxCheck = getMaxOneFill(BASE_LEN);
            while (wordToAdd > minCheck && addTo[(*d)] < maxCheck) {
                addTo[(*d)]++;
                wordToAdd--;
            }
            if (wordToAdd == minCheck) {
                return;//successfully added everything to previous
            }
            else { //stopped because ran out of space
                if(wordToAdd==minCheck+1) { //there was exactly one left so switch to literal before adding
                    wordToAdd = getZeroFill(BASE_LEN)-3;
                }
                addTo[++(*d)] = wordToAdd;
                return;
            }
        }
        else if(addT == ONE_LIT) {
            if(addTo[(*d)]<getMaxOneFill(BASE_LEN)) { //previous isn't maxed out so just add it
                addTo[(*d)]++;
                return;
            }
            else { //maxed out so can't consolidate
                addTo[++(*d)] = wordToAdd;
                return;
            }
        }
        else {
            addTo[++(*d)] = wordToAdd;
            return;
        }
    }
    else { // prev is lit of ones
        if(addT == ONE_LIT) {
            addTo[(*d)] = getOneFill(BASE_LEN);
            return;
        }
        else if (addT == ONE_RUN) {
            if (wordToAdd < getMaxOneFill(BASE_LEN)) { // not maxed out
                addTo[(*d)] = wordToAdd+1;
            }
            else { // maxed out so can't consolidate
                addTo[(*d)+1] = addTo[(*d)];
                addTo[(*d)++] = wordToAdd;
                return;
            }
        }
        else { // can't consolidate
            addTo[++(*d)] = wordToAdd;
            return;
        }
    }
}

word_32 fill_op_fill_wah(word_32, int, word_32 *, int *, word_32 (*)(word_32));

word_32 fillANDfillWAH(word_32 smallFill, int smallT, word_32 *bigFill, int *bigT)
{
    return fill_op_fill_wah(smallFill, smallT, bigFill, bigT, &getZeroFill);
}

word_32 fillORfillWAH(word_32 smallFill, int smallT, word_32 *bigFill, int *bigT)
{
    return fill_op_fill_wah(smallFill, smallT, bigFill, bigT, &getOneFill);
}

word_32 fill_op_fill_wah(word_32 smallFill, int smallT, word_32 *bigFill, int *bigT, word_32 (*ret_fill)(word_32))
{
    word_32 ret;
    word_32 sub = ((smallFill << 2) >> 2);
    if ((smallT == ONE_RUN && *bigT == ONE_RUN)
        || (smallT == ZERO_RUN && *bigT == ZERO_RUN)) {
        ret = smallFill;
    }
    else {
        ret = ret_fill(BASE_LEN) - 2 + sub;
    }
    *bigFill -= sub;
    if (((*bigFill << 2) >> 2) == 1) {
        if (*bigT == ZERO_RUN) {
            *bigFill = 0;//literal run of zeros
            *bigT = ZERO_LIT;
        }
        else {
            *bigFill = getZeroFill(BASE_LEN) - 3;
            *bigT = ONE_LIT;
        }
    }

    return ret;
}

word_32 fill_op_lit_wah(word_32*, int*, word_32);

word_32 fillORlitWAH(word_32 *fill, int *fillT, word_32 lit)
{
    return fill_op_lit_wah(fill, fillT, lit);
}

word_32 fillANDlitWAH(word_32 *fill, int *fillT, word_32 lit)
{
    return fill_op_lit_wah(fill, fillT, lit);
}

word_32 fill_op_lit_wah(word_32 *fill, int *fillT, word_32 lit)
{
    word_32 ret;
    if (*fillT == ZERO_RUN) {
        ret = 0;
    }
    else if (*fillT == ONE_RUN) {
        ret = getZeroFill(BASE_LEN) - 3;
    }
    else {
        ret = lit;
    }
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

/*
 * Performs an OR between 2 literals and returns the resulting word
 */
word_32 litORlitWAH(word_32 lit1, word_32 lit2)
{
    return lit1 | lit2;//just or them together
}

/*
 * Performs an AND between 2 literals and returns the resulting word
 */
word_32 litANDlitWAH(word_32 lit1, word_32 lit2)
{
    return lit1 & lit2;//just and them together
}
