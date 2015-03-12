#include <iostream>
#include <numeric>
#include <algorithm>
#include "gtest/gtest.h"
#include "btree/BTree.h"

TEST(BTree3Test, Add) {
	BTree tree3(3); // A B-Tree with minimum degree 3

	tree3.insert(1);
	EXPECT_EQ(tree3.to_str(), " [ 1 ]");
	tree3.insert(3);
	EXPECT_EQ(tree3.to_str(), " [ 1 3 ]");
	tree3.insert(7);
	EXPECT_EQ(tree3.to_str(), " [ 1 3 7 ]");
	tree3.insert(10);
	EXPECT_EQ(tree3.to_str(), " [ 1 3 7 10 ]");
	tree3.insert(11);
	EXPECT_EQ(tree3.to_str(), " [ 1 3 7 10 11 ]");
	tree3.insert(13);
	EXPECT_EQ(tree3.to_str(), " [ [ 1 3 ] 7 [ 10 11 13 ] ]");
	tree3.insert(14);
	EXPECT_EQ(tree3.to_str(), " [ [ 1 3 ] 7 [ 10 11 13 14 ] ]");
	tree3.insert(15);
	EXPECT_EQ(tree3.to_str(), " [ [ 1 3 ] 7 [ 10 11 13 14 15 ] ]");
}

TEST(BTree3Test, Remove) {
	BTree tree3(3); // A B-Tree with minimum degree 3

	tree3.insert(1);
	EXPECT_EQ(tree3.to_str(), " [ 1 ]");
	tree3.insert(3);
	EXPECT_EQ(tree3.to_str(), " [ 1 3 ]");
	tree3.remove(3);
	EXPECT_EQ(tree3.to_str(), " [ 1 ]");
	tree3.remove(1);
	EXPECT_EQ(tree3.to_str(), " [ ]");

	tree3.insert(7);
	EXPECT_EQ(tree3.to_str(), " [ 7 ]");
	tree3.insert(10);
	EXPECT_EQ(tree3.to_str(), " [ 7 10 ]");
	tree3.remove(7);
	EXPECT_EQ(tree3.to_str(), " [ 10 ]");
	tree3.remove(10);
	EXPECT_EQ(tree3.to_str(), " [ ]");
}

TEST(BTree3Test, EmptyRemove) {
	BTree tree3(3); // A B-Tree with minimum degree 3

	ASSERT_FALSE(tree3.remove(7));
}

TEST(BTree3Test, BigTest) {
	BTree tree3(3); // A B-Tree with minimum degree 3

	tree3.insert(1);
	tree3.insert(3);
	tree3.insert(7);
	tree3.insert(10);
	tree3.insert(11);
	tree3.insert(13);
	tree3.insert(14);
	tree3.insert(15);
	tree3.insert(18);
	tree3.insert(16);
	tree3.insert(19);
	tree3.insert(24);
	tree3.insert(25);
	tree3.insert(26);
	tree3.insert(21);
	tree3.insert(4);
	tree3.insert(5);
	tree3.insert(20);
	tree3.insert(22);
	tree3.insert(2);
	tree3.insert(17);
	tree3.insert(12);
	tree3.insert(6);
	EXPECT_EQ(tree3.to_str(),
			" [ [ [ 1 2 ] 3 [ 4 5 6 ] 7 [ 10 11 12 ] 13 [ 14 15 ] ] 16 [ [ 17 18 19 ] 20 [ 21 22 ] 24 [ 25 26 ] ] ]");

	tree3.remove(6);
	tree3.remove(13);
	tree3.remove(7);
	tree3.remove(4);
	tree3.remove(2);
	tree3.remove(16);
	EXPECT_EQ(tree3.to_str(),
			" [ [ 1 3 ] 5 [ 10 11 ] 12 [ 14 15 ] 17 [ 18 19 ] 20 [ 21 22 ] 24 [ 25 26 ] ]");

}

TEST(BTree4Test, BigTest) {
	BTree tree4(4); // A B-Tree with minimum degree 4

	tree4.insert(1);
	tree4.insert(3);
	tree4.insert(7);
	tree4.insert(10);
	tree4.insert(11);
	tree4.insert(13);
	tree4.insert(14);
	tree4.insert(15);
	tree4.insert(18);
	tree4.insert(16);
	tree4.insert(19);
	tree4.insert(24);
	tree4.insert(25);
	tree4.insert(26);
	tree4.insert(21);
	tree4.insert(4);
	tree4.insert(5);
	tree4.insert(20);
	tree4.insert(22);
	tree4.insert(2);
	tree4.insert(17);
	tree4.insert(12);
	tree4.insert(6);
	EXPECT_EQ(tree4.to_str(),
			" [ [ 1 2 3 4 5 6 7 ] 10 [ 11 12 13 14 ] 15 [ 16 17 18 19 20 ] 21 [ 22 24 25 26 ] ]");

	tree4.remove(6);
	tree4.remove(13);
	tree4.remove(7);
	tree4.remove(4);
	tree4.remove(2);
	tree4.remove(16);
	EXPECT_EQ(tree4.to_str(),
			" [ [ 1 3 5 ] 10 [ 11 12 14 ] 15 [ 17 18 19 20 ] 21 [ 22 24 25 26 ] ]");

}

TEST(BTree4Test, EmptyRemove) {
	BTree tree4(4); // A B-Tree with minimum degree 4

	ASSERT_FALSE(tree4.remove(7));
}

TEST(BTree4Test, RandomTest) {
	BTree tree4(4); // A B-Tree with minimum degree 4

	std::vector<int> keys(1000); // vector with 1000 ints.
	std::iota(keys.begin(), keys.end(), 0); // Fill with 0, 1, ..., 999.

	std::random_shuffle(std::begin(keys), std::end(keys)); // the first shuffle
	std::for_each(keys.begin(), keys.end(), [&tree4](int key) { // add
				tree4.insert(key);
			});

	ASSERT_TRUE(tree4.exist(7));
	ASSERT_TRUE(tree4.exist(17));
	ASSERT_TRUE(tree4.exist(77));

	std::random_shuffle(std::begin(keys), std::end(keys)); // the second shuffle
	std::for_each(keys.begin(), keys.end(), [&tree4](int key) { // remove
				tree4.remove(key);
			});

	ASSERT_FALSE(tree4.remove(7));
}

TEST(BTree2Test, BigTest) {
	BTree tree2(2); // A B-Tree with minimum degree 2

	tree2.insert(1);
	tree2.insert(3);
	tree2.insert(7);
	tree2.insert(10);
	tree2.insert(11);
	tree2.insert(13);
	tree2.insert(14);
	tree2.insert(15);
	tree2.insert(18);
	tree2.insert(16);
	tree2.insert(19);
	tree2.insert(24);
	tree2.insert(25);
	tree2.insert(26);
	tree2.insert(21);
	tree2.insert(4);
	tree2.insert(5);
	tree2.insert(20);
	tree2.insert(22);
	tree2.insert(2);
	tree2.insert(17);
	tree2.insert(12);
	tree2.insert(6);
	EXPECT_EQ(tree2.to_str(),
			" [ [ [ [ 1 2 ] 3 [ 4 ] 5 [ 6 7 ] ] 10 [ [ 11 12 ] 13 [ 14 ] ] ] 15 [ [ [ 16 17 ] 18 [ 19 ] ] 20 [ [ 21 22 ] 24 [ 25 26 ] ] ] ]");

	tree2.remove(6);
	tree2.remove(13);
	tree2.remove(7);
	tree2.remove(4);
	tree2.remove(2);
	tree2.remove(16);
	EXPECT_EQ(tree2.to_str(),
			" [ [ [ 1 3 ] 5 [ 10 11 ] ] 12 [ [ 14 ] 15 [ 17 ] 18 [ 19 ] ] 20 [ [ 21 22 ] 24 [ 25 26 ] ] ]");

}

GTEST_API_ int main(int argc, char **argv) {
	std::cout << "Running tests..." << std::endl;
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
