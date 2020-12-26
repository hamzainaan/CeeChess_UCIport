#include "stdio.h"
#include "defs.h"
#include "eval.h"

static int MaterialDraw(const S_BOARD *pos) {

	ASSERT(CheckBoard(pos));

    if (!pos->pceNum[wR] && !pos->pceNum[bR] && !pos->pceNum[wQ] && !pos->pceNum[bQ]) {
	  if (!pos->pceNum[bB] && !pos->pceNum[wB]) {
	      if (pos->pceNum[wN] < 3 && pos->pceNum[bN] < 3) {  return TRUE; }
	  } else if (!pos->pceNum[wN] && !pos->pceNum[bN]) {
	     if (abs(pos->pceNum[wB] - pos->pceNum[bB]) < 2) { return TRUE; }
	  } else if ((pos->pceNum[wN] < 3 && !pos->pceNum[wB]) || (pos->pceNum[wB] == 1 && !pos->pceNum[wN])) {
	    if ((pos->pceNum[bN] < 3 && !pos->pceNum[bB]) || (pos->pceNum[bB] == 1 && !pos->pceNum[bN]))  { return TRUE; }
	  }
	} else if (!pos->pceNum[wQ] && !pos->pceNum[bQ]) {
        if (pos->pceNum[wR] == 1 && pos->pceNum[bR] == 1) {
            if ((pos->pceNum[wN] + pos->pceNum[wB]) < 2 && (pos->pceNum[bN] + pos->pceNum[bB]) < 2)	{ return TRUE; }
        } else if (pos->pceNum[wR] == 1 && !pos->pceNum[bR]) {
            if ((pos->pceNum[wN] + pos->pceNum[wB] == 0) && (((pos->pceNum[bN] + pos->pceNum[bB]) == 1) || ((pos->pceNum[bN] + pos->pceNum[bB]) == 2))) { return TRUE; }
        } else if (pos->pceNum[bR] == 1 && !pos->pceNum[wR]) {
            if ((pos->pceNum[bN] + pos->pceNum[bB] == 0) && (((pos->pceNum[wN] + pos->pceNum[wB]) == 1) || ((pos->pceNum[wN] + pos->pceNum[wB]) == 2))) { return TRUE; }
        }
    }
  return FALSE;
}

int EvalPosition(S_BOARD *pos) {

	ASSERT(CheckBoard(pos));

	// test for drawn position before doing anything
	if(!pos->pceNum[wP] && !pos->pceNum[bP] && MaterialDraw(pos) == TRUE) {
		return 0;
	}

	int pce;
	int pceNum;
	int sq;
	int phase = totalPhase;
	int scoreMG, scoreEG;
	scoreMG = scoreEG = pos->material[WHITE] - pos->material[BLACK];
	int score;

	pce = wP;
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		ASSERT(SqOnBoard(sq));
		ASSERT(SQ64(sq)>=0 && SQ64(sq)<=63);

		scoreMG += PawnMG[SQ64(sq)];
		scoreEG += PawnEG[SQ64(sq)];

		if( (IsolatedMask[SQ64(sq)] & pos->pawns[WHITE]) == 0) {
			scoreMG += isolated_pawn[0];
			scoreEG += isolated_pawn[1];
		}

		if( (WhitePassedMask[SQ64(sq)] & pos->pawns[BLACK]) == 0) {
			scoreMG += passed_pawn[0][RanksBrd[sq]];
			scoreEG += passed_pawn[1][RanksBrd[sq]];
		}

	}

	pce = bP;
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		ASSERT(SqOnBoard(sq));
		ASSERT(MIRROR64(SQ64(sq))>=0 && MIRROR64(SQ64(sq))<=63);

		scoreMG -= PawnMG[MIRROR64(SQ64(sq))];
		scoreEG -= PawnEG[MIRROR64(SQ64(sq))];

		if( (IsolatedMask[SQ64(sq)] & pos->pawns[BLACK]) == 0) {
			scoreMG -= isolated_pawn[0];
			scoreEG -= isolated_pawn[1];
		}

		if( (BlackPassedMask[SQ64(sq)] & pos->pawns[WHITE]) == 0) {
			scoreMG -= passed_pawn[0][7 - RanksBrd[sq]];
			scoreEG -= passed_pawn[1][7 - RanksBrd[sq]];
		}
	}

	pce = wN;
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		ASSERT(SqOnBoard(sq));
		ASSERT(SQ64(sq)>=0 && SQ64(sq)<=63);

		scoreMG += KnightMG[SQ64(sq)];
		scoreEG += KnightEG[SQ64(sq)];
		phase -= 1;
	}

	pce = bN;
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		ASSERT(SqOnBoard(sq));
		ASSERT(MIRROR64(SQ64(sq))>=0 && MIRROR64(SQ64(sq))<=63);

		scoreMG -= KnightMG[MIRROR64(SQ64(sq))];
		scoreEG -= KnightEG[MIRROR64(SQ64(sq))];
		phase -= 1;
	}

	pce = wB;
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		ASSERT(SqOnBoard(sq));
		ASSERT(SQ64(sq)>=0 && SQ64(sq)<=63);

		scoreMG += BishopMG[SQ64(sq)];
		scoreEG += BishopEG[SQ64(sq)];
		phase -= 1;
	}

	pce = bB;
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		ASSERT(SqOnBoard(sq));
		ASSERT(MIRROR64(SQ64(sq))>=0 && MIRROR64(SQ64(sq))<=63);

		scoreMG -= BishopMG[MIRROR64(SQ64(sq))];
		scoreEG -= BishopEG[MIRROR64(SQ64(sq))];
		phase -= 1;
	}

	pce = wR;
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		ASSERT(SqOnBoard(sq));
		ASSERT(SQ64(sq)>=0 && SQ64(sq)<=63);
		ASSERT(FileRankValid(FilesBrd[sq]));

		scoreMG += RookMG[SQ64(sq)];
		scoreEG += RookEG[SQ64(sq)];

		if(!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			scoreMG += r_evalfile[1];
			scoreEG += r_evalfile[1];
		} else if(!(pos->pawns[WHITE] & FileBBMask[FilesBrd[sq]])) {
			scoreMG += r_evalfile[0];
			scoreEG += r_evalfile[0];
		}
		phase -= 2;
	}

	pce = bR;
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		ASSERT(SqOnBoard(sq));
		ASSERT(MIRROR64(SQ64(sq))>=0 && MIRROR64(SQ64(sq))<=63);
		ASSERT(FileRankValid(FilesBrd[sq]));

		scoreMG -= RookMG[MIRROR64(SQ64(sq))];
		scoreEG -= RookEG[MIRROR64(SQ64(sq))];

		if(!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			scoreMG -= r_evalfile[1];
			scoreEG -= r_evalfile[1];
		} else if(!(pos->pawns[BLACK] & FileBBMask[FilesBrd[sq]])) {
			scoreMG -= r_evalfile[0];
			scoreEG -= r_evalfile[0];
		}
		phase -= 2;
	}

	pce = wQ;
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		ASSERT(SqOnBoard(sq));
		ASSERT(SQ64(sq)>=0 && SQ64(sq)<=63);
		ASSERT(FileRankValid(FilesBrd[sq]));

		scoreMG += QueenMG[SQ64(sq)];
		scoreEG += QueenEG[SQ64(sq)];
		phase -= 4;
	}

	pce = bQ;
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		ASSERT(SqOnBoard(sq));
		ASSERT(SQ64(sq)>=0 && SQ64(sq)<=63);
		ASSERT(FileRankValid(FilesBrd[sq]));

		scoreMG -= QueenMG[MIRROR64(SQ64(sq))];
		scoreEG -= QueenEG[MIRROR64(SQ64(sq))];
		phase -= 4;
	}
	pce = wK;
	sq = pos->pList[pce][0];
	ASSERT(SqOnBoard(sq));
	ASSERT(SQ64(sq)>=0 && SQ64(sq)<=63);

	scoreMG += KingMG[SQ64(sq)];
	scoreEG += KingEG[SQ64(sq)];

	pce = bK;
	sq = pos->pList[pce][0];
	ASSERT(SqOnBoard(sq));
	ASSERT(MIRROR64(SQ64(sq))>=0 && MIRROR64(SQ64(sq))<=63);

	scoreMG -= KingMG[MIRROR64(SQ64(sq))];
	scoreEG -= KingEG[MIRROR64(SQ64(sq))];

	if(pos->pceNum[wB] >= 2) {
		scoreMG += bishop_pair[0];
		scoreEG += bishop_pair[1];
	}
	if(pos->pceNum[bB] >= 2) {
		scoreMG -= bishop_pair[0];
		scoreEG -= bishop_pair[1];
	}
	if(pos->side == WHITE) {
		scoreMG += 18;
		scoreEG += 9;
	}
	if(pos->side == BLACK) {
		scoreMG -= 18;
		scoreEG -= 9;
	}

	// calculating game phase and interpolating score values between phases
	phase = (phase * 256 + (totalPhase / 2)) / totalPhase;
	score = ((scoreMG * (256 - phase)) + (scoreEG * phase)) / 256;

	return pos->side == WHITE ? score : -score;

}
