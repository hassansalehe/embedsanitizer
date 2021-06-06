/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// Unit tests for defs.
//
////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include <thread>

#include "etsan/defs.h"

class DefsTestFixture : public ::testing::Test {
protected:

  const int num_threads = 5;

  DefsTestFixture() {
    TS.C.clear();
  }

  ~DefsTestFixture() {}
};

TEST_F(DefsTestFixture, checkThreadState) {
  ThreadState st;
  st.tid = 1;
  st.epoch = 0;
  st.C.reserve(st.tid +1);

  st.increment();
  EXPECT_EQ(1, st.epoch);
  EXPECT_EQ(1, st.C[st.tid]);

  st.C[st.tid] = 42;
  st.updateEpoch();
  ASSERT_EQ(st.C[st.tid], st.epoch); 
}

TEST_F(DefsTestFixture, checkUpdateThreadClocks) {
  for (int tid = 1; tid <= num_threads; ++tid) {
    TS.C[tid].epoch = tid << 24;
    TS.C[tid].tid = tid;
  }

  EXPECT_EQ(num_threads, TS.C.size());

  UpdateThreadClocks();

  for (const auto & thread_state : TS.C) {
    for (const auto & clock : thread_state.second.C) {
      if (thread_state.second.tid == TID(clock)) {
        // clock of a given thread has been incremented
        EXPECT_EQ(1, CLOCK(clock));
      }
      else {
        // clocks of other threads are set to 0
        EXPECT_EQ(0, CLOCK(clock));
      }
    }
  }
}

TEST_F(DefsTestFixture, checkGetStateNewThread) {
  constexpr int n_threads = 2;
  int tid1 = 1;
  auto thread1_state = getState(tid1);
  EXPECT_EQ(1, TS.C.size());
  EXPECT_EQ(1, CLOCK(thread1_state.epoch));

  int tid2 = 10;
  auto thread2_state = getState(tid2);
  EXPECT_EQ(n_threads, TS.C.size());
  EXPECT_EQ(1, CLOCK(thread2_state.epoch));
  EXPECT_EQ(n_threads, thread2_state.C.size());

  for (const auto & thread_state : TS.C) {
    for (const auto & clock : thread_state.second.C) {
      if (thread_state.second.tid == TID(clock)) {
        // clock of a given thread has been incremented
        EXPECT_EQ(1, CLOCK(clock));
      }
      else {
        // clocks of other threads are set to 0
        EXPECT_EQ(0, CLOCK(clock));
      }
    }
  }
}

// VectorClock related tests
TEST_F(DefsTestFixture, checkCreationOfNewVectorClock) {
  VectorClock VC;
  newVectorClock(VC, num_threads);

  EXPECT_EQ(num_threads, VC.size());
  for (int t = 0; t < num_threads; t++) {
    EXPECT_EQ(0, (VC.at(t) - (t << 24)));
  }
}

TEST_F(DefsTestFixture, checkExtendVectorClock) {
  VectorClock VC;
  newVectorClock(VC, num_threads);

  EXPECT_EQ(num_threads, VC.size());

  const auto double_num_threads = num_threads * 2;
  ExtendVectorClock(VC, double_num_threads);

  for (int t = 0; t < double_num_threads; t++) {
    EXPECT_EQ(0, (VC.at(t) - (t << 24)));
  }
}

TEST_F(DefsTestFixture, checkExtend2VectorClocks) {
  constexpr int size1 = 5;
  VectorClock VC1;
  newVectorClock(VC1, size1);

  constexpr int size2 = 4;
  VectorClock VC2;
  newVectorClock(VC2, size2);

  ExtendVectorClocks(VC2, VC1);
  auto max_size = std::max(size1, size2);
  EXPECT_EQ(max_size, VC1.size());
  EXPECT_EQ(max_size, VC2.size());

  for (int t = size1; t < max_size; t++) {
    EXPECT_EQ(0, (VC1.at(t) - (t << 24)));
  }

  for (int t = size2; t < max_size; t++) {
    EXPECT_EQ(0, (VC2.at(t) - (t << 24)));
  }
}
