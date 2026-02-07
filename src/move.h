// ------------------------------------ BITFISH ---------------------------------------

#pragma once
#include "type.h"
#include "position.h"
#include <vector>
#include <bitset>
#include <algorithm>


#define MOVE(from, to, moved, captured, flag) \
    (((from) & 0x3F) | (((to) & 0x3F) << 6) | \
     (((moved) & 0xF) << 12) | (((captured) & 0xF) << 16) | \
     (((flag) & 0xF) << 20))

#define NORMAL_MOVE(from, to, moved, captured) \
    MOVE(from, to, moved, captured, 0)

#define DOUBLE_PUSH_MOVE(from, to, moved) \
    MOVE(from, to, moved, NO_PIECE, MOVE_DOUBLE_PUSH_FLAG)

#define EN_PASSANT(from, to, moved, captured) \
    MOVE(from, to, moved, captured, MOVE_ENPASSANT_FLAG)

#define CASTLING_MOVE(from, to, moved) \
    MOVE(from, to, moved, NO_PIECE, MOVE_CASTLING_FLAG)

#define PROMO_MOVE(move, promo_flag) \
    (move | (promo_flag << 20))

#define FROM(move) \
    (move & 0x3F)

#define TO(move) \
    ((move >> 6) & 0x3F)

#define MOVED(move) \
    ((move >> 12) & 0xF)

#define CAPTURED(move) \
    ((move >> 16) & 0xF)

#define FLAG(move) \
    ((move >> 20) & 0xF)

#define PACK_GI(rule_50, ep, castling) \
    (castling & 0b1111) | ((ep & 127) << 4) | (rule_50 << 11)

#define CASTLING(gi) \
    (gi & 0b1111)

#define RULE_50(gi) \
    (gi >> 11)

#define EP(gi) \
    ((gi >> 4) & 127)

    

inline std::string move_to_string (Move move) {
    const std::array<char, 4> promo_chars = {
        'n',
        'b',
        'r',
        'q'
    };
    if (FLAG(move) >= MOVE_NPROMO_FLAG) 
    return square_to_str(Square(FROM(move))) + square_to_str(Square(TO(move))) + promo_chars[FLAG(move) - MOVE_NPROMO_FLAG];
    else 
    return square_to_str(Square(FROM(move))) + square_to_str(Square(TO(move)));
}

struct MoveList {
    std::array<Move, 256> list;
    int size = 0;

    inline void push_back (Move move) {
        list[size++] = move;
    }

    inline Move pop () {
        size --;
        return list[size];
    }

    inline Move peek () {
        return list[size - 1];
    }

    inline void clear () {
        size = 0;
    }

    inline Move operator[](int index) const {
        return list[index];
    }
    
    inline Move& operator[](int index) {
        return list[index];
    }

    // pv sort with mvv - lva
    inline void sort (Move move, Move killer1 = NO_MOVE, Move killer2 = NO_MOVE) {
        std::sort (list.begin(), list.begin() + size, [move, killer1, killer2](Move a, Move b){

            int score_a;
            int score_b;

            if (a == move) {
                score_a = 10000;
            } else if (a == killer1) {
                score_a = 9900;
            } else if (a == killer2) {
                score_a = 9800;
            } else if (CAPTURED(a) != NO_PIECE){
                score_a = std::abs(material[CAPTURED(a)]) - std::abs(material[MOVED(a)]);
                if (FLAG(a) >= MOVE_NPROMO_FLAG) score_a += promo_flag_bonus[FLAG(a) - MOVE_NPROMO_FLAG];
            } else {
                if (FLAG(a) >= MOVE_NPROMO_FLAG) score_a += promo_flag_bonus[FLAG(a) - MOVE_NPROMO_FLAG];
                else score_a = -10000;
            }

            if (b == move) {
                score_b = 10000;
            } else if (b == killer1) {
                score_b = 9900;
            } else if (b == killer2) {
                score_b = 9800;
            } else if (CAPTURED(b) != NO_PIECE){
                score_b = std::abs(material[CAPTURED(b)]) - std::abs(material[MOVED(b)]);
                if (FLAG(b) >= MOVE_NPROMO_FLAG) score_b += promo_flag_bonus[FLAG(b) - MOVE_NPROMO_FLAG];
            } else {
                if (FLAG(b) >= MOVE_NPROMO_FLAG) score_b += promo_flag_bonus[FLAG(b) - MOVE_NPROMO_FLAG];
                else score_b = -10000;
            }

            
            
            return score_a > score_b;
        });

        
    }

    inline void print () {
        for (int i = 0; i < size; i++) {
            std::cout << std::bitset<32>(list[i]) << ": " << move_to_string(list[i]) << ",\n";
        }
    }

};





