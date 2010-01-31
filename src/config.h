#pragma once

namespace Odin
{

const int QUEUE_MAX = 9;
const int RECOVERY_TIME = 1;
const int BLOCK_TIME = 1;
const float ACTIVATION_BIAS = 0.5f;
const float DRAIN = 0.9f;
const float MIN_ACTIVATION = 0.01f;
const float FIRE_MARGIN = 0.1f;
const float LEARN_RATE = 0.5f;

// Working on unsigned char [0..255] alphabet so far.
const int ALPHABET_SIZE = 256;

typedef int TNeuronID;
typedef int TTime; //internal time counter

} //Odin
