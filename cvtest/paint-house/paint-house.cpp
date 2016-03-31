// paint-house.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <iostream>
using namespace std;
/*
Paint House

27:51
重置
There are a row of n houses, each house can be painted with one of the three colors : red, blue or green.The cost of painting each house with a certain color is different.You have to paint all the houses such that no two adjacent houses have the same color.

The cost of painting each house with a certain color is represented by a n x 3 cost matrix.For example, costs[0][0] is the cost of painting house 0 with color red; costs[1][2] is the cost of painting house 1 with color green, and so on... Find the minimum cost to paint all houses.

您在真实的面试中是否遇到过这个题？ Yes
注意事项

All costs are positive integers.

样例
标签
相关题目
笔记
Given costs = [[14, 2, 11], [11, 14, 5], [14, 3, 10]] return 10*/
//dp[i][0]=costs[i-1][0] + min(dp[i-1][1],dp[i-1][2]);
//dp[i][1]=costs[i-1][1] + min(dp[i-1][0],dp[i-1][2]);
//dp[i][2]=costs[i-1][2] + min(dp[i-1][1],dp[i-1][0]);
void MakeNext(vector<int>& next,string p)
{
	next[0] = 0;
	fill_n();
	for (int i = 1; i < p.length();i++)
	{
	}
}
int minCost(vector<vector<int>>& costs)
{
	// Write your code here
	if (costs.size() == 0)
	{
		return 0;
	}
	vector<int> last(3, 0);
	vector<int> now(3, 0);
	for (int i = 1; i <= costs.size();i++)
	{
		now[0] = costs[i - 1][0] + min(last[1], last[2]);
		now[1] = costs[i - 1][1] + min(last[0], last[2]);
		now[2] = costs[i - 1][2] + min(last[1], last[0]);
		last = now;
	}
	last[0] = stoi("123");
	return min(now[0], min(now[1], now[2]));
}
int numWays(int n, int k) 
{
	// Write your code here
	vector<vector<int>> dp(n, vector<int>(k, 0));
	for (int i = 0; i < k;i++)
	{
		dp[0][k] = k;
	}
	for (int i = 1; i < n;i++)
	{
		for (int j = 1; j < k;j++)
		{
			dp[i][j] = (j - 1)*dp[i - 1][j] + dp[i][j - 1];
		}
	}
	return dp[n - 1][k - 1];
}
int minCostII(vector<vector<int>>& costs)
{
	// Write your code here
	int n = costs.size();
	if (n == 0) return 0;
	int k = costs[0].size();
	vector<int> now(k, 0);
	vector<int> last(k, 0);
	for (int i = 0; i < n; i++)
	{
		vector<int> temp = last;
		sort(temp.begin(), temp.end());
		for (int j = 0; j < k; j++)
		{
			int numdup = now[j];
			if (numdup == temp[0])
			{
				numdup = temp[1];
			}
			else
				numdup = temp[0];
			now[j] = numdup + costs[i][j];
		}
		for (int j = 0; j < k; j++)
		{
			last[j] = now[j];
		}
	}
	int res = INT_MAX;
	for (int j = 0; j < k; j++)
	{
		res = min(res, now[j]);
	}
	return res;
}

int numSquares(int n)
{
	// Write your code here
	int maxnum = sqrt(n);
	if (n%maxnum == 0)
	{
		return 1;
	}
	queue<int> que;
	vector<int> flag;
	flag.push_back(0);
	int i = 1;
	while (i <= maxnum)
	{
		que.push(i*i);
		flag.push_back(i*i);
		i *= i;
	}
	int count = que.size();
	int floor = 2;
	int lastcount = 0;
	while (!que.empty() || count != 0)
	{
		int temp = que.front();
		que.pop();
		count--;
		for (i = 0; i < flag.size();i++)
		{
			if (temp+flag[i] == n)
			{
				return floor;
			}
			que.push(temp + flag[i]);
			lastcount++;
		}
		if (count == 0)
		{
			count = lastcount;
			lastcount = 0;
			floor++;
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	cout << sizeof(int) << endl;
	system("pause");
	return 0;
}


inline void ConvDotProduct(const float *data,
	const float *weight, const float *biases, float *target,
	const int input_channels, const int input_stride,
	const int filter_num, const int filter_width, const int filter_height)
{
	memcpy(target, biases, filter_num*sizeof(float));
	const int filter_width_channels = filter_width * input_channels;
#if USE_SSE
	const __m128 *weight_m128 = (const __m128 *)weight;
	__m128 *target_m128 = (__m128 *)target;
	const int expend_length = 8;
	const int expend_loop = (filter_num >> 2) / expend_length;
	const int expend_remain = (filter_num >> 2) % expend_length;
#endif
	for (int y = 0; y < filter_height; y++)
	{
		for (int x = 0; x < filter_width_channels; x++)
		{
			const float v = data[x];
			if (v != 0)
			{
#if USE_SSE
				__m128 pixel_value = _mm_set_ps1(v);
				__m128 *r_base = target_m128;
				const __m128 *r_weight = weight_m128;
				for (int i = 0; i < expend_loop; i++)
				{
					r_base[0] = _mm_add_ps(r_base[0], _mm_mul_ps(pixel_value, r_weight[0]));
					r_base[1] = _mm_add_ps(r_base[1], _mm_mul_ps(pixel_value, r_weight[1]));
					r_base[2] = _mm_add_ps(r_base[2], _mm_mul_ps(pixel_value, r_weight[2]));
					r_base[3] = _mm_add_ps(r_base[3], _mm_mul_ps(pixel_value, r_weight[3]));
					r_base[4] = _mm_add_ps(r_base[4], _mm_mul_ps(pixel_value, r_weight[4]));
					r_base[5] = _mm_add_ps(r_base[5], _mm_mul_ps(pixel_value, r_weight[5]));
					r_base[6] = _mm_add_ps(r_base[6], _mm_mul_ps(pixel_value, r_weight[6]));
					r_base[7] = _mm_add_ps(r_base[7], _mm_mul_ps(pixel_value, r_weight[7]));
					r_weight += expend_length;
					r_base += expend_length;
				}
				for (int i = 0; i < expend_remain; i++)
					r_base[i] = _mm_add_ps(r_base[i], _mm_mul_ps(pixel_value, r_weight[i]));
#else
				for (int i = 0; i < filter_num; i++)
				{
					target[i] += v * weight[i];
				}
#endif
			}
#if USE_SSE
			weight_m128 += filter_num >> 2;
#else
			weight += filter_num;
#endif
		}
		data += input_stride;
	}
}
