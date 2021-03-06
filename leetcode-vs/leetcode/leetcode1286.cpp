#include <numeric>
#include <future>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <set>
#include <queue>
#include <assert.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>
#include <functional>
#include <mutex>
#include <string>
#include <array>
#include "common.h"

using namespace std;


//1286. Iterator for Combination
class CombinationIterator {
    string s;
    int n, len;
    vector<int> pos;
    bool valid = true;
public:
    CombinationIterator(string s, int n) : pos(len, 0), s(s), n(n), len(s.size()) {
        sort(s.begin(), s.end());
        for (int i = 0; i < n; ++i)
        {
            pos[i] = 1;
        }
    }

    string next() {
        string ret;
        ret.clear();
        for (int i = 0; i < len; ++i)
        {
            if (pos[i])
            {
                ret.push_back(s[i]);
            }
        }

        int i = len - 1;
        int cnt = 0;
        valid = true;
        if (pos[i] == 1)
        {
            for (; i >= 0 && pos[i] == 1; --i)
            {
                cnt++;
            }
            if (cnt == n)
            {
                valid = false;
            }
            for (int j = len - 1; j > i; --j)
            {
                pos[j] = 0;
            }
            for (; i >= 0; --i)
            {
                if (pos[i] == 1)
                {
                    pos[i] = 0;
                    pos[i + 1] = 1;
                    for (int j = 0; j < cnt; ++j)
                    {
                        pos[i + 2 + j] = 1;
                    }
                    break;
                }
            }
        }
        else
        {
            for (; i >= 0 && pos[i] == 0; --i);
            pos[i] = 0;
            pos[i + 1] = 1;
        }
        return ret;
    }

    bool hasNext() {
        return valid;
    }
};


/*
//1320. Minimum Distance to Type a Word Using Two Fingers
int memo[36][36][310];

class Solution {
public:
    typedef pair<int, int> PTS;
    vector<pair<int, int>> char2pts;
    int dist(PTS a, PTS b)
    {
        return abs(a.first - b.first) + abs(a.second - b.second);
    }
    int dfs(PTS a, PTS b, int u, string& s)
    {
        if (u == s.size()) return 0;
        int& ans = memo[a.first * 6 + a.second][b.first * 6 + b.second][u];
        if (ans != -1) return ans;
        auto nx = char2pts[s[u] - 'A'];
        ans = INT_MAX;
        int cost;
        if (a.first != 5)
        {
            cost = dist(a, nx);
        }
        else
        {
            cost = 0;
        }
        ans = min(ans, dfs(nx, b, u + 1, s) + cost);

        if (b.first != 5)
        {
            cost = dist(b, nx);
        }
        else
        {
            cost = 0;
        }
        ans = min(ans, dfs(a, nx, u + 1, s) + cost);
        return ans;
    }

    int minimumDistance(string word) {
        memset(memo, -1, sizeof(memo));
        vector<pair<int, int>> pts(26);
        int n = 5, m = 6;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                int c = i * m + j;
                if (c >= 26) break;
                pts[c] = { i, j };
            }
        }
        char2pts = pts;
        return dfs({ 5, 5 }, { 5, 5 }, 0, word);
    }
};
    //int minimumDistance(string word) {
    //    vector<int> dp(26);
    //    int res = 0, save = 0, n = word.size();
    //    for (int i = 0; i < n - 1; ++i) {
    //        int b = word[i] - 'A', c = word[i + 1] - 'A';
    //        for (int a = 0; a < 26; ++a)
    //            dp[b] = max(dp[b], dp[a] + d(b, c) - d(a, c));
    //        save = max(save, dp[b]);
    //        res += d(b, c);
    //    }
    //    return res - save;
    //}

    //int d(int a, int b) {
    //    return abs(a / 6 - b / 6) + abs(a % 6 - b % 6);
    //}
*/

//1357. Apply Discount Every n Orders
class Cashier {
public:
    vector<int> product_prices;
    int every_n, discount;
    int cur = 0;
    Cashier(int n, int discount, vector<int>& products, vector<int>& prices) : every_n(n), discount(discount), product_prices(201) {
        int len = products.size();
        for (int i = 0; i < len; ++i)
        {
            product_prices[products[i]] = prices[i];
        }
    }

    double getBill(vector<int> product, vector<int> amount) {
        double ans = 0;
        int len = product.size();
        for (int i = 0; i < len; ++i)
        {
            ans += product_prices[product[i]] * amount[i];
        }
        if ((++cur) % every_n == 0)
        {
            ans *= (100 - discount) / 100.0;
        }
        return ans;
    }
};

//1381. Design a Stack With Increment Operation
class CustomStack {
public:
    vector<int> stk, op;
    int maxSize, size;
    CustomStack(int maxSize) : maxSize(maxSize), size(0)
    {
        stk.reserve(maxSize);
        op.reserve(maxSize);
    }

    void push(int x)
    {
        if (size + 1 > maxSize) return;
        stk.push_back(x);
        op.push_back(0);
        size++;
    }

    int pop()
    {
        if (size == 0) return -1;
        size -= 1;

        auto ret = stk.back();
        stk.pop_back();

        auto add = op.back();
        op.pop_back();

        if (size > 0)
        {
            auto nx = op.back();
            op.pop_back();
            op.push_back(nx + add);
        }
        return ret + add;
    }

    void increment(int k, int val) {
        if (size == 0) return;
        op[min(size - 1, k - 1)] += val;
    }
};

//1656. Design an Ordered Stream
class OrderedStream {
    vector<string> pr;
    int len = 0;
    int ptr = 1;
public:
    OrderedStream(int n) : pr(n + 1) {
        len = n;
    }

    vector<string> insert(int id, string value) {
        pr[id] = value;
        vector<string> ans;
        if (id == ptr)
        {
            for (int i = ptr; i <= len; ++i)
            {
                if (!pr[i].empty())
                {
                    ans.push_back(pr[i]);
                }
                else break;
            }
            ptr += ans.size();
        }
        return ans;
    }
};


class Solution
{
public:
    //1288. Remove Covered Intervals
    int removeCoveredIntervals(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());
        int last = 0, ans = 1;
        for (int i = 0; i < intervals.size(); i++) {
            if (intervals[last][0] <= intervals[i][0] &&
                intervals[i][1] <= intervals[last][1])
                continue;
            ans++;
            last = i;
        }
        return ans;
    }

    //1289. Minimum Falling Path Sum II
    int minFallingPathSum(vector<vector<int>>& a) {
        int n = a.size(), m = a[0].size();
        vector<vector<int>> dp(n, vector<int>(m, INT_MAX));
        for (int j = 0; j < m; ++j) dp[0][j] = a[0][j];
        for (int i = 1; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                for (int k = 0; k < m; ++k)
                {
                    if (j == k) continue;
                    dp[i][j] = min(dp[i][j], dp[i - 1][k] + a[i][j]);
                }
            }
        }
        int ans = INT_MAX;
        for (int j = 0; j < m; ++j)
        {
            ans = min(ans, dp[n - 1][j]);
        }
        return ans;
    }
    
    //1287. Element Appearing More Than 25% In Sorted Array
    int findSpecialInteger(vector<int>& a) {
        int n = a.size();
        for (int i = 0; i < n; )
        {
            int v = a[i];
            int cnt = 0;
            while (i < n && a[i] == v)
            {
                cnt++;
                i++;
            }
            if (cnt > n / 4.0)
                return v;
        }
        return -1;
    }


    //1290. Convert Binary Number in a Linked List to Integer
	int getDecimalValue(ListNode* head) {
		int ans = 0;
		while (head)
		{
			ans = ans * 2 + head->val;
			head = head->next;
		}
		return ans;
	}

    vector<int> solve_len_1291(int n, int low, int high)
    {
        vector<int> ans;
        for (int i = 1; i + n - 1 < 10; ++i)
        {
            int t = 0;
            for (int j = 0; j < n; ++j)
            {
                t = t * 10 + i + j;
            }
            if (low <= t && t <= high)
                ans.push_back(t);
        }
        return ans;
    }
    //1291. Sequential Digits
    vector<int> sequentialDigits(int low, int high) {
        vector<int> ans;
        for (int l = 1; l <= 9; ++l)
        {
            for (auto& e : solve_len_1291(l, low, high))
            {
                ans.push_back(e);
            }
        }
        return ans;
    }

    //1292. Maximum Side Length of a Square with Sum Less than or Equal to Threshold
	int maxSideLength(vector<vector<int>>& mat, int t) {
		int n = mat.size(), m = mat[0].size();
		NumMatrix nm(mat);
		int l = 0, r = max(n, m) + 1;
		while (l < r)
		{
			int mid = (l + r) / 2;
			bool valid = false;
			for (int i = 0; i + mid - 1 < n; ++i)
			{
				for (int j = 0; j + mid - 1 < m; ++j)
				{
					int val = nm.sumRegion(i, j, i + mid - 1, j + mid - 1);
					if (val <= t)
					{
						valid = true;
					}
				}
			}
			if (!valid)
			{
				r = mid;
			}
			else
			{
				l = mid + 1;
			}
		}
		return l - 1;
	}

    /*
    //1293. Shortest Path in a Grid with Obstacles Elimination
    int dr[] = { 0, 1, 0, -1 };
    int dc[] = { 1, 0, -1, 0 };

	int const maxn = 40 + 1;
	int dp[maxn][maxn][maxn * maxn];
	class Solution {
	public:
		int n, m;

        int dfs(int i, int j, int left, vector<vector<int>>& g)
        {
            if (i == n - 1 && j == m - 1) return 0;
            int& ans = dp[i][j][left];
            if (ans != -1) return ans;
            ans = INT_MAX / 4;
            for (int d = 0; d < 4; ++d)
            {
                int ni = i + dr[d], nj = j + dc[d];
                if (0 <= ni && ni < n && 0 <= nj && nj < m)
                {
                    if (g[ni][nj] == 1 && left > 0)
                    {
                        ans = min(ans, dfs(ni, nj, left - 1, g) + 1);
                    }
                    else if (g[ni][nj] == 0)
                    {
                        ans = min(ans, dfs(ni, nj, left, g) + 1);
                    }
                }
            }
            return ans;
        }
        int shortestPath(vector<vector<int>>& grid, int k) {
            if (grid[0][0] == 1)
            {
                if (k == 0) return -1;
                k--;
                grid[0][0] = 0;
            }
            memset(dp, -1, sizeof(dp));
            n = grid.size(), m = grid[0].size();
            int ret = dfs(0, 0, k, grid);
            if (ret == INT_MAX / 4) return -1;
            return ret;
        }
    };
    */

	//1295. Find Numbers with Even Number of Digits
	int findNumbers(vector<int>& a) {
		int ans = 0;
		for (auto e : a)
		{
			int len = 0;
			while (e)
			{
				len++;
				e /= 10;
			}
			ans += (len % 2 == 0);
		}
		return ans;
	}

	//1296. Divide Array in Sets of K Consecutive Numbers
	bool isPossibleDivide(vector<int>& nums, int k) {
		map<int, int> cnt;
		for (auto& e : nums)
			cnt[e] ++;
		while (!cnt.empty())
		{
			int u = cnt.begin()->first;
			for (int i = 0; i < k; ++i)
			{
				int v = i + u;
				if (cnt.count(v))
				{
					if (--cnt[v] == 0)
					{
						cnt.erase(v);
					}
				}
				else
				{
					return false;
				}
			}
		}
		return true;
	}

	//1297. Maximum Number of Occurrences of a Substring
	int maxFreq(string s, int maxLetters, int minSize, int maxSize) {
		unordered_map<string, int> cnt;
		int ans = 0;
		int diff = 0, i = 0, j = 0;
		int char_count[256] = { 0 };
		for (; j < minSize; ++j) {
			if (++char_count[s[j]] == 1) {
				diff++;
			}
		}
		do {
			if (diff <= maxLetters) {
				ans = max(ans, ++cnt[s.substr(i, minSize)]);
			}
			if (j == s.length()) break;
			if (--char_count[s[i]] == 0) {
				diff--;
			}
			if (++char_count[s[j]] == 1) {
				diff++;
			}
			i++; j++;
		} while (1);
		return ans;
	}


	//1298. Maximum Candies You Can Get from Boxes
	int maxCandies(vector<int>& status, vector<int>& candies, vector<vector<int>>& keys, vector<vector<int>>& containedBoxes, vector<int>& initialBoxes) {
		queue<int> q;
		for (auto& e : initialBoxes) q.push(e);
		int ans = 0;
		int n = status.size();

		vector<int> vis(n);     // hasCandies
		vector<int> hasKey(n);  // left keys
		vector<int> hasBox(n);  // need key

		while (!q.empty())
		{
			auto u = q.front(); q.pop();
			hasBox[u] = 1;

			if (!vis[u])
			{
				if (status[u] == 1)
				{
					ans += candies[u];
					vis[u] = 1;
				}
				else
				{
					hasBox[u] = 1;
				}
			}

			for (auto& box : containedBoxes[u])
			{
				hasBox[box] = 1;
			}

			for (auto& key : keys[u])
			{
				hasKey[key] = 1;
			}

			for (int box = 0; box < n; ++box)
			{
				if (hasBox[box] && status[box] && vis[box] == 0)
				{
					ans += candies[box];
					q.push(box);
					vis[box] = 1;
				}
			}

			for (int key = 0; key < n; ++key)
			{
				if (hasBox[key] && hasKey[key] && vis[key] == 0)
				{
					vis[key] = 1;
					q.push(key);

					ans += candies[key];
					hasBox[key] = 0;
				}
			}
		}
		return ans;
	}

    //1299. Replace Elements with Greatest Element on Right Side
    vector<int> replaceElements(vector<int>& arr) {
        int mx = -1;
        for (int i = arr.size() - 1; i >= 0; --i)
        {
            int t = arr[i];
            arr[i] = mx;
            mx = max(mx, t);
        }
        return arr;
    }

    //1300. Sum of Mutated Array Closest to Target
    int findBestValue(vector<int>& a, int t) {
        int n = a.size();
        int l = 0, r = 1e5 + 1;

        int ret = 0;

        while (l < r)
        {
            int m = (l + r) / 2;
            int sum = 0;
            for (int i = 0; i < n; ++i)
            {
                sum += min(m, a[i]);
            }

            if (sum >= t)
            {
                r = m;
            }
            else
            {
                l = m + 1;
            }
        }
        int ans_less = 0, ans_big = 0;
        for (auto& e : a)
        {
            ans_less += min(e, l - 1);
            ans_big += min(e, l);
        }

        if (abs(ans_less - t) <= abs(ans_big - t))
        {
            return l - 1;
        }
        return l;
    }

    //1301. Number of Paths with Max Score
    vector<int> pathsWithMaxScore(vector<string>& board) {
        int n = board.size(), m = board[0].size(), mod = 1e9 + 7;
        vector<pair<int, int>> dp(m + 1);
        dp[0] = { 0, 1 };
        for (int i = 0; i < n; i++) {
            auto last = dp;
            dp[0] = { 0, 0 };
            for (int j = 0; j < m; j++) {
                if (board[i][j] == 'X') {
                    dp[j + 1] = { 0, 0 };
                    continue;
                }
                auto [a0, a1] = dp[j];
                auto [b0, b1] = last[j];
                auto [c0, c1] = last[j + 1];
                if (!a1 && !b1 && !c1)
                    dp[j + 1] = { 0, 0 };
                else {
                    int64_t maxsum = max({ a0, b0, c0 }), ways = 0;
                    ways += a0 == maxsum ? a1 : 0;
                    ways += b0 == maxsum ? b1 : 0;
                    ways += c0 == maxsum ? c1 : 0;;
                    maxsum += isdigit(board[i][j]) ? board[i][j] - '0' : 0;
                    dp[j + 1] = { maxsum % mod, ways % mod };
                }
            }
        }
        return { dp[m].first, dp[m].second };
    }


    void dfs_1302(int d, TreeNode* u, vector<int> &dep)
    {
        if (!u) return;
        if (dep.size() < d + 1) dep.push_back(u->val);
        else dep[d] += u->val;
        dfs_1302(d + 1, u->left, dep);
        dfs_1302(d + 1, u->right, dep);
    }
    //1302. Deepest Leaves Sum
    int deepestLeavesSum(TreeNode* root) {
        vector<int> dep;
        dfs_1302(0, root, dep);
        return dep.back();
    }

    //1304. Find N Unique Integers Sum up to Zero
    vector<int> sumZero(int n) {
        vector<int> ans;
        if (n % 2) ans.push_back(0);
        for (int i = 0; i < n / 2; ++i)
        {
            ans.push_back(i + 1);
            ans.push_back(-i - 1);
        }
        return ans;
    }


    void dfs_1305(TreeNode* u, vector<int>& a)
    {
        if (!u) return;
        dfs_1305(u->left, a);
        a.push_back(u->val);
        dfs_1305(u->right, a);
    }
    //1305. All Elements in Two Binary Search Trees
    vector<int> getAllElements(TreeNode* root1, TreeNode* root2) {
        vector<int> a, b;
        dfs_1305(root1, a);
        dfs_1305(root2, b);
        vector<int> c(a.size() + b.size());
        merge(a.begin(), a.end(), b.begin(), b.end(), c.begin());
        return c;
    }
    
    //1306. Jump Game III
    bool canReach(vector<int>& a, int st) {
        int n = a.size();
        queue<int> q;
        vector<bool> vis(n);
        q.push(st);
        vis[st] = 1;
        if (a[st] == 0) return true;

        while (!q.empty())
        {
            int size = q.size();
            while (size--)
            {
                auto u = q.front();
                q.pop();
                if (a[u] == 0) return true;
                if (u + a[u] < n)
                {
                    if (!vis[u + a[u]])
                    {
                        vis[u + a[u]] = 1;
                        q.push(u + a[u]);
                    }
                }
                if (u - a[u] >= 0)
                {
                    if (!vis[u - a[u]])
                    {
                        vis[u - a[u]] = 1;
                        q.push(u - a[u]);
                    }
                }
            }
        }
        return false;
    }


    int calc_left_5298(vector<pair<char, int>>& left, int carry, vector<int>& test)
    {
        int value_left = carry;
        for (auto& e : left)
        {
            value_left += test[e.first - 'A'] * e.second;
        }
        return value_left;
    }

    bool calc_5298(vector<pair<char, int>>& left, int carry, char right, vector<int>& test)
    {
        if (calc_left_5298(left, carry, test) % 10 == test[right - 'A'])
        {
            return true;
        }
        return false;
    }


    void dfs_1307(int u, vector<pair<char, int>>& left, int carry, char right, vector<int>& test,
        vector<vector<int>>& out, vector<int>& pre, vector<bool>& vis)
    {
        if (u == left.size())
        {
            if (pre[right - 'A'] != -1)
            {
                test[right - 'A'] = pre[right - 'A'];
                if (calc_5298(left, carry, right, test))
                {
                    out.push_back(test);
                }
            }
            else
            {
                for (int i = 0; i < 10; ++i)
                {
                    if (vis[i] == 0)
                    {
                        vis[i] = 1;
                        test[right - 'A'] = i;
                        if (calc_5298(left, carry, right, test))
                        {
                            out.push_back(test);
                        }
                        vis[i] = 0;
                    }
                }
            }
            return;
        }


        if (pre[left[u].first - 'A'] != -1)
        {
            test[left[u].first - 'A'] = pre[left[u].first - 'A'];
            dfs_1307(u + 1, left, carry, right, test, out, pre, vis);
        }
        else
        {
            for (int i = 0; i < 10; ++i)
            {
                if (vis[i] == 0)
                {
                    vis[i] = 1;
                    test[left[u].first - 'A'] = i;
                    dfs_1307(u + 1, left, carry, right, test, out, pre, vis);
                    vis[i] = 0;
                }
            }
        }
    }

    vector<vector<int>> generate_5298(vector<pair<char, int>>& left, int carry, char right, vector<int>& pre)
    {
        vector<int> test(26, -1);
        vector<vector<int>> out;

        vector<bool> vis(10);
        for (int i = 0; i < 26; ++i)
        {
            if (pre[i] != -1) vis[pre[i]] = 1;
        }

        dfs_1307(0, left, carry, right, test,
            out, pre, vis);
        return out;
    }

    bool dfs_1307(vector<int>& pre, int pos, int carry, vector<string>& ws, string& res)
    {
        if (pos == res.size()) return true;
        vector<int> cnt(26);
        for (int i = 0; i < ws.size(); ++i)
        {
            if (pos < ws[i].size())
            {
                cnt[ws[i][pos] - 'A'] ++;
            }
        }
        vector<pair<char, int>> left;
        for (char i = 'A'; i <= 'Z'; ++i)
        {
            if (cnt[i - 'A'])
            {
                left.push_back({ i, cnt[i - 'A'] });
            }
        }
        char right = res[pos];
        auto test = generate_5298(left, carry, right, pre);
        for (auto& e : test)
        {
            vector<int> er;
            for (int i = 0; i < 26; ++i)
            {
                if (e[i] == -1) continue;
                if (pre[i] == -1) er.push_back(i);
                pre[i] = e[i];
            }
            int value_left = calc_left_5298(left, carry, e);
            if (dfs_1307(pre, pos + 1, value_left / 10, ws, res))
                return true;
            for (auto& i : er)
            {
                pre[i] = -1;
            }
        }
        return false;
    }

    //1307. Verbal Arithmetic Puzzle
    bool isSolvable(vector<string>& words, string result) {
        for (auto& e : words) reverse(e.begin(), e.end());
        reverse(result.begin(), result.end());
        vector<int> pre(26, -1);
        return dfs_1307(pre, 0, 0, words, result);
    }

    //1309. Decrypt String from Alphabet to Integer Mapping
    string freqAlphabets(string s) {
        string ans;
        int n = s.size();
        for (int i = 0; i < n; )
        {

            if (i + 2 < n && s[i + 2] == '#')
            {
                ans.push_back((s[i] - '0') * 10 + s[i + 1] - '0' - 10 + 'j');
                i += 3;
            }
            else
            {
                ans.push_back(s[i] - '0' - 1 + 'a');
                i++;
            }
        }
        return ans;
    }

    //1310. XOR Queries of a Subarray
    vector<int> xorQueries(vector<int>& a, vector<vector<int>>& qs) {
        int n = a.size();
        vector<vector<int>> sums(n + 1, vector<int>(32));
        for (int mask = 0; mask < 31; ++mask)
        {
            for (int i = 0; i < n; ++i)
            {
                sums[i + 1][mask] = sums[i][mask] + ((a[i] & (1 << mask)) != 0);
            }
        }
        int m = qs.size();
        vector<int> ans;
        for (auto& e : qs)
        {
            int l = e[0], r = e[1];
            int val = 0;
            for (int i = 0; i < 31; ++i)
            {
                if ((sums[r + 1][i] - sums[l][i]) & 1)
                {
                    val += (1 << i);
                }
            }
            ans.push_back(val);
        }
        return ans;
    }

    //1311. Get Watched Videos by Your Friends
    vector<string> watchedVideosByFriends(vector<vector<string>>& watchedVideos, vector<vector<int>>& g, int id, int level) {
        queue<int> q;
        map<string, int> freq;
        q.push(id);
        int n = g.size();
        vector<bool> vis(n);
        vis[id] = 1;
        while (!q.empty() && level)
        {
            freq.clear();
            int size = q.size();
            while (size--)
            {
                auto u = q.front();
                q.pop();
                for (auto& v : g[u])
                {
                    if (!vis[v])
                    {
                        vis[v] = 1;
                        q.push(v);
                        for (auto& e : watchedVideos[v])
                        {
                            freq[e] ++;
                        }
                    }
                }
            }
            if (--level == 0)
            {
                vector<pair<int, string>> ord;
                for (auto& e : freq)
                {
                    ord.push_back({ e.second, e.first });
                }
                sort(ord.begin(), ord.end());
                vector<string> ans;
                for (auto& e : ord)
                {
                    ans.push_back(e.second);
                }
                return ans;
            }
        }
        return {};
    }

    //1312. Minimum Insertion Steps to Make a String Palindrome
    int minInsertions(string s) {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n));
        for (int i = 0; i < n; ++i)
        {
            dp[i][i] = 1;
            if (i + 1 < n)
            {
                dp[i][i + 1] = 1 + (s[i] == s[i + 1]);
            }
        }
        for (int l = 3; l <= n; ++l)
        {
            for (int i = 0, j = i + l - 1; j < n; ++i, ++j)
            {
                if (s[i] == s[j])
                {
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                }
                else
                {
                    dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
                }
            }
        }
        return n - dp[0][n - 1];
    }
    
    //1323. Maximum 69 Number
    int maximum69Number(int num) {

        int mask = 1;
        while (mask < num)
        {
            mask *= 10;
        }

        int ans = num;

        for (; mask >= 1; mask /= 10)
        {
            int val = num / mask % 10;
            if (val == 6)
            {
                ans += 3 * mask;
                break;
            }
        }
        return ans;
    }

    vector<string> split_1324(string& s)
    {
        vector<string> ans;
        string u;
        int n = s.size();
        for (int i = 0; i < n; ++i)
        {
            while (i < n && s[i] == ' ') ++i;
            while (i < n && isalpha(s[i]))
            {
                u.push_back(s[i++]);
            }
            ans.push_back(u);
            u.clear();
        }
        return ans;
    }

    //1324. Print Words Vertically
    vector<string> printVertically(string s) {
        auto tokens = split_1324(s);
        int n = tokens.size();
        int maxlen = 0;
        for (auto& e : tokens) maxlen = max(maxlen, static_cast<int>(e.size()));
        vector<string> br(maxlen, string(n, ' '));
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < tokens[i].size(); ++j)
            {
                br[j][i] = tokens[i][j];
            }
        }
        vector<string> ans;
        for (auto& e : br)
        {
            ans.push_back("");

            int len = e.size() - 1;
            while (len >= 0 && e[len] == ' ') len--;

            for (int i = 0; i <= len; ++i)
                ans.back().push_back(e[i]);
        }
        return ans;
    }

    //1325. Delete Leaves With a Given Value
    TreeNode* removeLeafNodes(TreeNode* root, int target) {
        if (!root) return nullptr;
        root->left = removeLeafNodes(root->left, target);
        root->right = removeLeafNodes(root->right, target);
        if (root->left == nullptr && root->right == nullptr)
        {
            if (root->val == target) return nullptr;
        }
        return root;
    }

    //1326. Minimum Number of Taps to Open to Water a Garden
    int minTaps(int n, vector<int>& ranges) {
        int m = ranges.size();
        vector<pair<int, int>> intervals(m);
        for (int i = 0; i < m; ++i)
        {
            intervals[i] = { max(i - ranges[i], 0), min(n, i + ranges[i]) };
        }
        sort(intervals.begin(), intervals.end(),
            [](const pair<int, int>& a, const pair<int, int>& b)
            {
                if (a.first != b.first) return a.first < b.first;
                return a.second > b.second;
            }
        );
        int ans = 0;
        int cur = 0;
        int nx = 0;
        for (int i = 0; i < m; ++i)
        {
            int s = intervals[i].first, e = intervals[i].second;
            if (cur < s)
            {
                if (s <= nx)
                {
                    ans++;
                    cur = nx;
                    nx = max(nx, e);
                }
                else return -1;
            }
            else if (s <= cur)
            {
                nx = max(nx, e);
            }
            if (cur >= n) return ans;
        }
        if (cur >= n) return  ans;
        else if (nx >= n) return ans + 1;
        return -1;
    }

    //1313. Decompress Run-Length Encoded List
    vector<int> decompressRLElist(vector<int>& nums) {
        int n = nums.size();

        int len = 0;
        for (int i = 0; i * 2 + 1 < n; ++i)
        {
            int a = nums[i * 2];
            len += a;
        }

        vector<int> ans;
        ans.reserve(len);

        for (int i = 0; i * 2 + 1 < n; ++i)
        {
            int a = nums[i * 2], b = nums[i * 2 + 1];
            for (int j = 0; j < a; ++j)
            {
                ans.push_back(b);
            }
        }
        return ans;
    }

    //1314. Matrix Block Sum
    vector<vector<int>> matrixBlockSum(vector<vector<int>>& a, int k) {
        NumMatrix nm(a);
        int n = a.size(), m = a[0].size();
        vector<vector<int>> ans(n, vector<int>(m));
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                ans[i][j] = nm.sumRegion(max(i - k, 0), max(j - k, 0), min(i + k, n - 1), min(j + k, m - 1));
            }
        }
        return ans;
    }

    //1315. Sum of Nodes with Even - Valued Grandparent
    int dfs_1315(TreeNode* u, bool gp, bool p)
    {
        if (!u) return 0;
        int ans = 0;
        ans += dfs_1315(u->left, p, u->val % 2 == 0);
        ans += dfs_1315(u->right, p, u->val % 2 == 0);
        if (gp) ans += u->val;
        return ans;
    }
    int sumEvenGrandparent(TreeNode* root) {
        return dfs_1315(root, false, false);
    }

    //1316. Distinct Echo Substrings
    int distinctEchoSubstrings(string text) {
        partial_sum_str psum(text);
        vector<vector<int>> pos(26);
        int n = text.size();
        unordered_set<int64_t> vis;
        for (int i = 0; i < n; ++i)
        {
            for (auto j : pos[text[i] - 'a'])
            {
                int l = i - j;
                if (i + l > n) continue;
                int64_t hash = psum.sum(j, j + l);
                if (hash == psum.sum(i, i + l))
                {
                    vis.insert(hash);
                }
            }
            pos[text[i] - 'a'].push_back(i);
        }
        return vis.size();
    }


    bool check_1317(int n)
    {
        while (n)
        {
            if (n % 10 == 0) return false;
            n /= 10;
        }
        return true;
    }
    //1317. Convert Integer to the Sum of Two No-Zero Integers
    vector<int> getNoZeroIntegers(int n) {
        for (int i = 1; i < n; ++i)
        {
            if (check_1317(i) && check_1317(n - i))
            {
                return { i, n - i };
            }
        }
        return {};
    }

    //1318. Minimum Flips to Make a OR b Equal to c
    int minFlips(int a, int b, int c) {

        int m[2][2][2];
        m[0][0][0] = 0;
        m[0][0][1] = 1;

        m[0][1][0] = 1;
        m[0][1][1] = 0;

        m[1][0][0] = 1;
        m[1][0][1] = 0;

        m[1][1][0] = 2;
        m[1][1][1] = 0;

        int ans = 0;
        for (int i = 0; i < 31; ++i)
        {
            ans += m[a & 1][b & 1][c & 1];
            a >>= 1;
            b >>= 1;
            c >>= 1;
        }

        return ans;
    }

    void dfs_1319(int u, vector<bool>& vis, vector<vector<int>>& g)
    {
        vis[u] = true;
        for (auto& v : g[u])
        {
            if (!vis[v])
            {
                dfs_1319(v, vis, g);
            }
        }
    }

    //1319. Number of Operations to Make Network Connected
    int makeConnected(int n, vector<vector<int>>& gx) {
        if (gx.size() < n - 1) return -1;
        vector<vector<int>> g(n);
        for (auto& e : gx)
        {
            int u = e[0], v = e[1];
            g[u].push_back(v);
            g[v].push_back(u);
        }
        vector<bool> vis(n);
        int ans = 0;
        for (int i = 0; i < n; ++i)
        {
            if (!vis[i])
            {
                ans++;
                dfs_1319(i, vis, g);
            }
        }
        return ans - 1;
    }

    //1329. Sort the Matrix Diagonally
    vector<vector<int>> diagonalSort(vector<vector<int>>& a) {
        int n = a.size(), m = a[0].size();
        vector<pair<int, int>> pos;
        for (int i = 0; i < n; ++i)
        {
            pos.push_back({ i, 0 });
        }
        for (int j = 1; j < m; ++j)
        {
            pos.push_back({ 0, j });
        }
        vector<int> tmp;
        for (auto [sx, sy] : pos)
        {
            tmp.clear();

            for (int i = sx, j = sy; i < n && j < m; ++i, ++j)
            {
                tmp.push_back(a[i][j]);
            }

            sort(tmp.begin(), tmp.end());
            auto it = tmp.begin();
            for (int i = sx, j = sy; i < n && j < m; ++i, ++j)
            {
                a[i][j] = *(it++);
            }
        }
        return a;
    }

    //1331. Rank Transform of an Array
    vector<int> arrayRankTransform(vector<int>& a) {
        int n = a.size();
        vector<pair<int, int>> pos(n);
        for (int i = 0; i < n; ++i)
        {
            pos[i] = { a[i], i };
        }
        sort(pos.begin(), pos.end());
        int rank = 1;
        for (int i = 0; i < n; ++i)
        {
            auto [val, pos_val] = pos[i];
            if (i - 1 >= 0 && val != pos[i - 1].first) rank++;
            a[pos_val] = rank;
        }
        return a;
    }

    //1328. Break a Palindrome
    string breakPalindrome(string s) {
        if (s.size() == 1) return "";
        int n = s.size();
        for (int i = 0; i < n / 2; ++i)
        {
            if (s[i] != 'a')
            {
                s[i] = 'a';
                return s;
            }
        }
        s[n - 1] = 'b';
        return s;
    }

    //1332. Remove Palindromic Subsequences
    int removePalindromeSub(string s) {
        int n = s.size();
        if (n == 0) return 0;
        if (n == 1) return 1;
        bool f = true;
        for (int i = 0, j = n - 1; i < j; ++i, --j)
        {
            if (s[i] != s[j])
            {
                f = false;
                break;
            }
        }
        if (f) return 1;
        return 2;
    }

    //1333. Filter Restaurants by Vegan-Friendly, Price and Distance
    vector<int> filterRestaurants(vector<vector<int>>& restaurants, int veganFriendly, int maxPrice, int maxDistance) {
        vector<pair<int, int>> ans;
        for (auto& res : restaurants)
        {
            int id = res[0], rate = res[1], vegan = res[2],
                price = res[3], dist = res[4];
            if (veganFriendly == false || veganFriendly == true && vegan)
            {
                if (price <= maxPrice && dist <= maxDistance)
                {
                    ans.push_back({ rate, id });
                }
            }
        }
        sort(ans.begin(), ans.end(), greater<pair<int, int>>());
        vector<int> ret;
        for (auto& e : ans)
        {
            ret.push_back(e.second);
        }
        return ret;
    }

    //1334. Find the City With the Smallest Number of Neighbors at a Threshold Distance
    int findTheCity(int n, vector<vector<int>>& edges, int distanceThreshold) {
        const int maxn = INT_MAX / 4;
        vector<vector<int>> g(n, vector<int>(n, maxn));
        for (auto& e : edges)
        {
            int u = e[0], v = e[1], cost = e[2];
            g[u][v] = min(cost, g[u][v]);
            g[v][u] = min(cost, g[v][u]);
        }
        for (int k = 0; k < n; ++k)
        {
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    if (i == j) continue;
                    g[i][j] = min(g[i][k] + g[k][j], g[i][j]);
                }
            }
        }
        int ans = maxn;
        int ret = 0;
        for (int i = 0; i < n; ++i)
        {
            int cnt = 0;
            for (int j = 0; j < n; ++j)
            {
                if (g[i][j] <= distanceThreshold)
                {
                    cnt++;
                }
            }
            if (ans >= cnt)
            {
                ans = cnt;
                ret = i;
            }
        }
        return ret;
    }

    //1335. Minimum Difficulty of a Job Schedule
    int minDifficulty(vector<int>& a, int d) {

        const int maxn = INT_MAX / 4;

        int n = a.size();

        vector<int> dp(n, 0), pre(n, 0);
        pre[0] = a[0];
        for (int i = 1; i < n; ++i)
        {
            pre[i] = max(pre[i - 1], a[i]);
        }
        for (int i = 0; i < d - 1; ++i)
        {
            fill(dp.begin(), dp.end(), maxn);
            for (int j = 0; j < n; ++j)
            {
                int mx = 0;
                for (int k = j + 1; k < n; ++k)
                {
                    mx = max(mx, a[k]);
                    dp[k] = min(pre[j] + mx, dp[k]);
                }
            }
            swap(dp, pre);
        }
        return pre[n - 1] == maxn ? -1 : pre[n - 1];
    }
    //1330. Reverse Subarray To Maximize Array Value
    int maxValueAfterReverse(vector<int>& a) {
        int ans = 0;
        int n = a.size();
        for (int i = 1; i < n; ++i)
        {
            ans += abs(a[i] - a[i - 1]);
        }
        int mi = INT_MAX, mx = INT_MIN;
        int add = 0;
        for (int i = 1; i < n; ++i)
        {
            mi = min(mi, max(a[i], a[i - 1]));
            mx = max(mx, min(a[i], a[i - 1]));
            add = max(add, (mx - mi) * 2);
        }
        for (int i = 1; i < n; ++i)
        {
            int diff = abs(a[i] - a[i - 1]);
            int t1 = abs(a[0] - a[i]) - diff;
            int t2 = abs(a[i - 1] - a[n - 1]) - diff;
            add = max(add, t1);
            add = max(add, t2);
        }
        return ans + add;
    }

    //1341. The K Weakest Rows in a Matrix
    vector<int> kWeakestRows(vector<vector<int>>& mat, int k) {
        int n = mat.size(), m = mat[0].size();
        vector<vector<int>> a(n, vector<int>(2));
        for (int i = 0; i < n; ++i)
        {
            int cnt = 0;
            for (int j = 0; j < m; ++j)
            {
                if (mat[i][j])
                {
                    cnt++;
                }
                else
                {
                    break;
                }
            }
            a[i] = { cnt, i };
        }
        sort(a.begin(), a.end());
        vector<int> ans;
        for (int i = 0; i < k; ++i) ans.push_back(a[i][2]);
        return ans;
    }

    //1342. Reduce Array Size to The Half
    int minSetSize(vector<int>& arr) {
        map<int, int> cnt;
        int n = arr.size();
        for (auto& e : arr) cnt[e] ++;
        vector<int> num;
        auto it = cnt.begin();
        while (it != cnt.end())
        {
            num.push_back(it->second);
            ++it;
        }
        int m = num.size();
        sort(num.begin(), num.end());
        int u = 0;
        for (int i = m - 1; i >= 0; --i)
        {
            u += num[i];
            if (u >= n / 2)
            {
                return m - i;
            }
        }
        return m;
    }

    long long dfs_1343(TreeNode* u, long long sum, long long& ans)
    {
        if (!u) return 0;
        auto left = dfs_1343(u->left, sum, ans);
        auto right = dfs_1343(u->right, sum, ans);

        ans = max(ans, left * (sum - left));
        ans = max(ans, right * (sum - right));

        return left + right + u->val;
    }

    //1343. Maximum Product of Splitted Binary Tree
    int maxProduct(TreeNode* root) {
        const int mod = 1e9 + 7;
        function<int(TreeNode*)> getsum;
        getsum = [&](TreeNode* u) {
            if (!u) return 0;
            return getsum(u->left) + getsum(u->right) + u->val;
        };
        int sum = getsum(root);
        long long ans = 0;
        dfs_1343(root, sum, ans);
        return ans % mod;
    }

    //1344. Jump Game V
    int maxJumps(vector<int>& a, int d) {
        int n = a.size();
        vector<int> left(n, -1);
        stack<int> stk;

        for (int i = n - 1; i >= 0; --i)
        {
            while (!stk.empty() && a[stk.top()] < a[i])
            {
                left[stk.top()] = i;
                stk.pop();
            }
            stk.push(i);
        }
        while (!stk.empty())
        {
            left[stk.top()] = -1;
            stk.pop();
        }

        vector<int> right(n, -1);
        for (int i = 0; i < n; ++i)
        {
            while (!stk.empty() && a[stk.top()] < a[i])
            {
                right[stk.top()] = i;
                stk.pop();
            }
            stk.push(i);
        }
        while (!stk.empty())
        {
            right[stk.top()] = -1;
            stk.pop();
        }
        queue<int> q;
        for (int i = 0; i < n; ++i)
        {
            q.push(i);
        }
        int ans = 0;
        vector<int> vis(n);
        while (!q.empty())
        {
            int size = q.size();
            ans++;
            //fill(vis.begin(), vis.end(), 0);
            while (size--)
            {
                auto u = q.front();
                q.pop();
                auto left_p = left[u];
                auto right_p = right[u];
                if (left_p != -1 && abs(left_p - u) <= d && vis[left_p] < ans)
                {
                    q.push(left_p);
                    vis[left_p]++;
                }
                if (right_p != -1 && abs(right_p - u) <= d && vis[right_p] < ans)
                {
                    q.push(right_p);
                    vis[right_p]++;
                }
            }
        }
        return ans;
    }

    //1342. Number of Steps to Reduce a Number to Zero
    int numberOfSteps(int num) {
        return num == 0 ? 0 : log2(num) + bitset<32>(num).count();
    }

    //1343. Number of Sub-arrays of Size K and Average Greater than or Equal to Threshold
    int numOfSubarrays(vector<int>& arr, int k, int threshold) {
        int ans = 0;
        int n = arr.size();
        int sum = 0;
        for (int i = 0; i < n; ++i)
        {
            sum += arr[i];
            if (i >= k - 1)
            {
                if (sum >= k * threshold)
                {
                    ans++;
                }
                sum -= arr[i - k + 1];
            }
        }
        return ans;
    }

    //1344. Angle Between Hands of a Clock
    double angleClock(int hour, int minutes) {
        double s_hour = (hour + minutes / 60.0) * (360 / 12.0);
        double s_minute = minutes * (360 / 60.0);
        double ans = fabs(s_hour - s_minute);
        return min(360 - ans, ans);
    }

    //1345. Jump Game IV
    int minJumps(vector<int>& a) {
        int n = a.size();
        if (n == 1) return 0;

        unordered_map<int, vector<int>> a_index;
        for (int i = 0; i < n; ++i)
        {
            a_index[a[i]].push_back(i);
        }

        vector<bool> vis(n);
        queue<int> q;
        q.push(0);
        vis[0] = 1;
        unordered_set<int> value;
        int ans = 0;
        while (!q.empty())
        {
            int size = q.size();
            while (size--)
            {
                auto i = q.front(); q.pop();
                if (i == n - 1) return ans;
                if (i - 1 >= 0 && !vis[i - 1])
                {
                    q.push(i - 1);
                    vis[i - 1] = 1;
                }
                if (i + 1 < n && !vis[i + 1])
                {
                    q.push(i + 1);
                    vis[i + 1] = 1;
                }
                if (!value.count(a[i]))
                {
                    value.insert(a[i]);
                    for (auto& idx : a_index[a[i]])
                    {
                        if (!vis[idx])
                        {
                            q.push(idx);
                            vis[idx] = 1;
                        }
                    }
                }
            }
            ans++;
        }
        return n - 1;
    }

    //1346. Check If N and Its Double Exist
    bool checkIfExist(vector<int>& a) {
        int n = a.size();
        map<int, int> cnt;
        for (auto& e : a) cnt[e] ++;
        for (auto it = cnt.begin(); it != cnt.end(); ++it)
        {
            int i = it->first, j = it->second;
            if (i == 0)
            {
                if (j > 1) return true;
            }
            else if (cnt.count(i * 2))
            {
                return true;
            }
        }
        return false;
    }

    //1347. Minimum Number of Steps to Make Two Strings Anagram
    int minSteps(string s, string t) {
        vector<int> cnt_s(26), cnt_t(26);
        for (auto& c : s) cnt_s[c - 'a'] ++;
        for (auto& c : t) cnt_t[c - 'a'] ++;
        vector<int> diff(26);
        int ans = 0;
        for (int i = 0; i < 26; ++i) if (cnt_t[i] - cnt_s[i] > 0) ans += cnt_t[i] - cnt_s[i];
        return ans;
    }

    /*
//1348. Tweet Counts Per Frequency
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <cstdio>
#include <functional>

using namespace std;
using namespace __gnu_pbds;

typedef tree<
int,
    null_type, 
        less<int>, 
        rb_tree_tag, 
        tree_order_statistics_node_update > order_set;


    class TweetCounts {
    public:
        map<string, order_set> tweets;
        TweetCounts() {

        }

        void recordTweet(string tweetName, int time) {
            tweets[tweetName].insert(time);
        }

        vector<int> getTweetCountsPerFrequency(string freq, string tweetName, int startTime, int endTime) {
            int diff = 0;
            if (freq == "minute") diff = 60;
            else if (freq == "hour") diff = 60 * 60;
            else diff = 24 * 60 * 60;
            vector<int> ans;
            auto& cnt = tweets[tweetName];
            int last = cnt.order_of_key(startTime);
            for (int cur = startTime; cur <= endTime; cur += diff)
            {
                int rank = cnt.order_of_key(min(cur + diff, endTime + 1));
                ans.push_back(rank - last);
                last = rank;
            }
            return ans;
        }
    };
    */

    //1349. Maximum Students Taking Exam
    int maxStudents(vector<vector<char>>& seats) {
        int n = seats.size(), m = seats[0].size();
        vector<int> val(n);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                val[i] = val[i] * 2 + (seats[i][j] == '.');
            }
        }
        int const M = (1 << m);
        vector<vector<int>> dp(n + 1, vector<int>(M, -1));
        dp[0][0] = 0;
        for (int i = 1; i <= n; ++i)
        {
            int cur = val[i - 1];
            for (int j = 0; j < M; ++j)
            {
                if ((cur & j) == j && (j & (j >> 1)) == 0)
                {
                    for (int k = 0; k < M; ++k)
                    {
                        if ((j & (k >> 1)) == 0 && (j & (k << 1)) == 0 && dp[i - 1][k] != -1)
                        {
                            dp[i][j] = max(dp[i][j], dp[i - 1][k] + __builtin_popcount(j));
                        }
                    }
                }
            }
        }
        return *max_element(dp[n].begin(), dp[n].end());
    }

    //1356. Sort Integers by The Number of 1 Bits
    vector<int> sortByBits(vector<int>& arr) {
        sort(arr.begin(), arr.end(), [](const int lhs, const int rhs) {
            int cnt_l = __builtin_popcount(lhs), cnt_r = __builtin_popcount(rhs);
            if (cnt_l == cnt_r) return lhs < rhs;
            return cnt_l < cnt_r;
            });
        return arr;
    }

    //1358. Number of Substrings Containing All Three Characters
    int numberOfSubstrings(string s) {
        int ans = 0;
        int len = s.size();
        vector<int> cnt(3);
        int valid = 0;
        for (int i = 0, j = 0; j < len; ++j)
        {
            if ((++cnt[s[j] - 'a']) == 1)
            {
                valid++;
            }
            while (i < j && valid == 3)
            {
                if (cnt[s[i] - 'a'] == 1) break;
                cnt[s[i++] - 'a'] --;
            }
            if (valid == 3) ans += i + 1;
        }
        return ans;
    }

    //1359. Count All Valid Pickup and Delivery Options
    int countOrders(int n) {
        int const mod = 1e9 + 7;
        long long inv2 = fastpow(2, mod - 2, mod);
        long long ans = 1;
        for (long long i = n + n; i >= 2; i -= 2)
        {
            ans = (ans * (i * (i - 1)) % mod) * inv2 % mod;
        }
        return ans % mod;
    }

    //1361. Validate Binary Tree Nodes
    bool validateBinaryTreeNodes(int n, vector<int>& leftChild, vector<int>& rightChild) {
        vector<vector<int>> g(n);
        vector<int> inorder(n);

        for (int i = 0; i < n; ++i)
        {
            int left = leftChild[i], right = rightChild[i];
            if (left != -1)
            {
                g[i].push_back(left);
                inorder[left] ++;
            }
            if (right != -1)
            {
                g[i].push_back(right);
                inorder[right] ++;
            }
        }
        int root = -1;
        for (int i = 0; i < n; ++i)
        {
            if (inorder[i] == 0)
            {
                if (root == -1)
                {
                    root = i;
                }
                else
                {
                    return false;
                }
            }
            else if (inorder[i] > 1) return false;
        }
        if (root == -1) return false;
        stack<int> stk;
        stk.push(root);
        vector<int> vis(n);
        vis[root] = 1;
        while (!stk.empty())
        {
            auto u = stk.top();
            stk.pop();
            for (auto& e : g[u])
            {
                stk.push(e);
                vis[e] = 1;
            }
        }
        for (int i = 0; i < n; ++i) if (!vis[i]) return false;
        return true;
    }

    pair<int, int> solve_1362(int n)
    {
        int v = sqrt(n);
        for (int i = v; i >= 1; --i)
        {
            if (n % i == 0)
            {
                return { i, n / i };
            }
        }
        return { 1, n };
    }
    //1362. Closest Divisors
    vector<int> closestDivisors(int num) {
        auto left = solve_1362(num + 1), right = solve_1362(num + 2);
        if (abs(left.first - left.second) < abs(right.first - right.second))
        {
            return { left.first, left.second };
        }
        else
        {
            return { right.first, right.second };
        }
    }

    int days_1360(string date) {
        int y = stoi(date.substr(0, 4)), m = stoi(date.substr(5, 2)), d = stoi(date.substr(8));
        if (m < 3) {
            m += 12;
            y -= 1;
        }
        return 365 * y + y / 4 + y / 400 - y / 100 + d + (153 * m + 8) / 5;
    }
    //1360. Number of Days Between Two Dates
    int daysBetweenDates(string date1, string date2) {
        return abs(days_1360(date1) - days_1360(date2));
    }

    //1363. Largest Multiple of Three
    string largestMultipleOfThree(vector<int>& digits) {
        sort(rbegin(digits), rend(digits));
        int sum = 0, cnt[11] = {};
        for (auto& d : digits) {
            sum += d;
            ++cnt[d];
        }
        function<bool(int)> done = [&](int d) {
            if (cnt[d]) {
                sum -= d;
                --cnt[d];
                for (int i = digits.size() - 1; i >= 0; --i) {
                    if (digits[i] == d) {
                        digits.erase(i + begin(digits));
                        break;
                    }
                }
            }
            return sum % 3 == 0;
        };
        if (sum % 3 == 1) {
            if (!done(1) && !done(4) && !done(7)) {
                done(2) || done(2) || done(5) || done(5) || done(8) || done(8);
            }
        }
        else if (sum % 3 == 2) {
            if (!done(2) && !done(5) && !done(8)) {
                done(1) || done(1) || done(4) || done(4) || done(7) || done(7);
            }
        }
        string ans;
        for (auto& d : digits) {
            ans += to_string(d);
        }
        return ans[0] == '0' ? "0" : ans;
    }

    //1365. How Many Numbers Are Smaller Than the Current Number
    vector<int> smallerNumbersThanCurrent(vector<int>& a) {
        vector<int> ans = a;
        sort(a.begin(), a.end());
        int n = a.size();
        for (int i = 0; i < n; ++i)
        {
            auto x = lower_bound(a.begin(), a.end(), ans[i]) - a.begin();
            ans[i] = x;
        }
        return ans;
    }

    //1366. Rank Teams by Votes
    string rankTeams(vector<string>& votes) {
        vector<vector<int>> order(26, vector<int>(26));
        int n = votes.size();
        vector<bool> has(26);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < votes[i].size(); ++j)
            {
                order[votes[i][j] - 'A'][j] ++;
                has[votes[i][j] - 'A'] = true;
            }
        }
        vector<int> idx(26);
        for (int i = 0; i < 26; ++i) idx[i] = i;
        sort(idx.begin(), idx.end(), [&](int i, int j) {
            if (order[i] == order[j]) return i > j;
            return order[i] < order[j];
            });
        string ans;
        for (int i = 26 - 1; i >= 0; --i)
        {
            int c = idx[i];
            if (!has[c]) break;
            ans.push_back(c + 'A');
        }
        return ans;
    }

    bool checkdown_1367(ListNode* u, TreeNode* v)
    {
        if (!u) return true;
        if (!v) return false;
        if (u->val != v->val) return false;
        return checkdown_1367(u->next, v->left) || checkdown_1367(u->next, v->right);
    }
    //1367. Linked List in Binary Tree
    bool isSubPath(ListNode* head, TreeNode* root) {
        if (!root) return false;
        if (head->val == head->val)
        {
            if (checkdown_1367(head, root)) return true;
        }
        return isSubPath(head, root->left) || isSubPath(head, root->right);
    }

    int findSmallest_1370(int i, vector<int>& cnt)
    {
        for (int j = i + 1; j < cnt.size(); ++j)
        {
            if (cnt[j])
            {
                return j;
            }
        }
        return -1;
    }
    int findLargest_1370(int i, vector<int>& cnt)
    {
        for (int j = i - 1; j >= 0; --j)
        {
            if (cnt[j])
            {
                return j;
            }
        }
        return -1;
    }
    //1370. Increasing Decreasing String
    string sortString(string s) {
        vector<int> cnt(26);
        for (auto& c : s) cnt[c - 'a'] ++;
        string ans;
        int tot = 0;
        while (true)
        {
            auto mi = findSmallest_1370(-1, cnt);

            if (mi == -1) break;
            cnt[mi] --;

            ans.push_back(mi + 'a');
            int idx = mi;
            while ((idx = findSmallest_1370(idx, cnt)) != -1)
            {
                ans.push_back(idx + 'a');
                cnt[idx] --;
            }

            int mx = findLargest_1370(26, cnt);
            if (mx == -1) break;
            cnt[mx] --;
            ans.push_back(mx + 'a');
            idx = mx;
            while ((idx = findLargest_1370(idx, cnt)) != -1)
            {
                ans.push_back(idx + 'a');
                cnt[idx] --;
            }
        }
        return ans;
    }

    //1371. Find the Longest Substring Containing Vowels in Even Counts
    int findTheLongestSubstring(string s) {
        vector<int> chs(26, -1);
        string vowel = "aeiou";
        for (int i = 0; i < vowel.size(); ++i)
        {
            chs[vowel[i] - 'a'] = i;
        }

        vector<int> last(1 << vowel.size(), -2);
        int const mask = (1 << vowel.size()) - 1;
        int n = s.size();
        vector<int> cnt(n);
        int ans = 0;
        last[0] = -1;
        for (int i = 0; i < s.size(); ++i)
        {
            if (i != 0) cnt[i] = cnt[i - 1];
            if (chs[s[i] - 'a'] != -1)
            {
                cnt[i] ^= (1 << chs[s[i] - 'a']);
                cnt[i] &= mask;
                if (last[cnt[i]] == -2) last[cnt[i]] = i;
            }
            ans = max(ans, i - last[cnt[i]]);
        }
        return ans;
    }

    //1374. Generate a String With Characters That Have Odd Counts
    string generateTheString(int n) 
    {
        if (n & 1) return string(n, 'a');
        else
        {
            int d = n / 2;
            if (d & 1) return string(d, 'a') + string(n - d, 'b');
            else return string(n / 2 + 1, 'a') + string(n - n / 2 - 1, 'b');
        }
    }

    //1375. Bulb Switcher III
    int numTimesAllBlue(vector<int>& light) {
        int n = light.size();
        BIT bit(n + 1);
        int ans = 0;
        int mx = -1;
        for (auto& m : light)
        {
            bit.add(m, 1);
            mx = max(mx, m);
            if (mx == bit.query(mx)) ans++;
        }
        return ans;
    }

    int dfs_1376(int u, vector<int>& time, vector<vector<int>>& g)
    {
        if (g[u].empty()) return 0;
        int ans = 0;
        for (auto& v : g[u])
        {
            ans = max(ans, dfs_1376(v, time, g) + time[u]);
        }
        return ans;
    }

    //1376. Time Needed to Inform All Employees
    int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
        vector<vector<int>> g(n + 1);
        for (int i = 0; i < n; ++i)
        {
            int mi = manager[i];
            if (mi != -1)
            {
                g[mi].push_back(i);
            }
        }
        return dfs_1376(headID, informTime, g);
    }

    //1372. Longest ZigZag Path in a Binary Tree
    int longestZigZag(TreeNode* root) {
        return max(dfs_1372(root->left, 1), dfs_1372(root->right, 0)) - 1;
    }
    int dfs_1372(TreeNode* root, int order)
    {
        if (!root)
        {
            return 0;
        }
        int ans = 0;
        if (order == 1)
        {
            ans = max(ans, dfs_1372(root->right, 0) + 1);
            ans = max(ans, dfs_1372(root->left, 1));
        }

        if (order == 0)
        {
            ans = max(ans, dfs_1372(root->left, 1) + 1);
            ans = max(ans, dfs_1372(root->right, 0));
        }
    }

    //1380. Lucky Numbers in a Matrix
    vector<int> luckyNumbers(vector<vector<int>>& a) {
        int n = a.size(), m = a[0].size();
        vector<int> ans;
        for (int i = 0; i < n; ++i)
        {
            int mi = 0;
            for (int j = 1; j < m; ++j)
            {
                if (a[i][j] < a[i][mi])
                {
                    mi = j;
                }
            }
            bool f = true;
            for (int k = 0; f && k < n; ++k)
            {
                if (a[k][mi] > a[i][mi])
                {
                    f = false;
                }
            }
            if (f) ans.push_back(a[i][mi]);
        }
        return ans;
    }

    //1379. Find a Corresponding Node of a Binary Tree in a Clone of That Tree
    TreeNode* getTargetCopy(TreeNode* original, TreeNode* cloned, TreeNode* target) {
        if (!original) return nullptr;
        if (original == target)
        {
            return cloned;
        }
        TreeNode* ret;
        ret = getTargetCopy(original->right, cloned->right, target);
        if (ret) return ret;
        return getTargetCopy(original->left, cloned->left, target);
    }

    TreeNode* build_1382(int i, int j, vector<int>& a)
    {
        if (i > j) return nullptr;
        if (i == j) return new TreeNode(a[i]);
        int m = (i + j) / 2;
        auto ret = new TreeNode(a[m]);
        ret->left = build_1382(i, m - 1, a);
        ret->right = build_1382(m + 1, j, a);
        return ret;
    }

    //1382. Balance a Binary Search Tree
    TreeNode* balanceBST(TreeNode* root) {
        vector<int> a;
        stack<TreeNode*> stk;
        auto cur = root;
        while (cur || !stk.empty())
        {
            if (cur == nullptr)
            {
                cur = stk.top();
                stk.pop();
                a.push_back(cur->val);
                cur = cur->right;
            }
            else
            {
                stk.push(cur);
                cur = cur->left;
            }
        }
        return build_1382(0, a.size() - 1, a);
    }

    //1624. Largest Substring Between Two Equal Characters
    int maxLengthBetweenEqualCharacters(string s) {
        map<char, int> pre;
        int ans = -1;
        for (int i = 0; i < s.size(); ++i)
        {
            char c = s[i];
            if (pre.count(c))
            {
                ans = max(ans, i - pre[c] - 1);
            }
            else
            {
                pre[c] = i;
            }
        }
        return ans;
    }

    //1626. Best Team With No Conflicts
    int bestTeamScore(vector<int>& scores, vector<int>& ages) {
        int n = scores.size();
        vector<pair<int, int>> a(n);
        for (int i = 0; i < n; ++i)
        {
            a[i].first = ages[i];
            a[i].second = scores[i];
        }
        sort(a.begin(), a.end());
        vector<int> dp(n);
        dp[0] = a[0].second;
        for (int i = 0; i < n; ++i)
        {
            dp[i] = a[i].second;
            for (int j = i - 1; j >= 0; --j)
            {
                if (a[j].second <= a[i].second)
                {
                    dp[i] = max(dp[i], dp[j] + a[i].second);
                }
            }
        }
        return *max_element(dp.begin(), dp.end());
    }

    int solve_1625(int st, string& s, int a)
    {
        int d = 0;
        vector<int> order(10);
        int c = s[st] - '0';
        int cnt = 0;
        for (int i = 0; order[c] == 0; ++i)
        {
            order[c] = cnt;
            c = (c + a) % 10;
            cnt += a;
        }
        order[c] = cnt;
        for (int i = 0; i < 10; ++i)
        {
            if (order[i]) return order[i];
        }
        return -1;
    }
    void get_1625(int odd, int even, string& ns)
    {
        for (int i = 0; i < ns.size(); ++i)
        {
            int v = ns[i] - '0';
            ns[i] = (v + (i % 2 ? odd : even)) % 10 + '0';
        }
    }

    //1625. Lexicographically Smallest String After Applying Operations
    string findLexSmallestString(string s, int a, int b) {
        string ans;
        int n = s.size();
        vector<int> vis(n);
        for (int offset = 0; !vis[offset]; offset = (offset + b) % n)
        {
            vis[offset] = 1;
            string ns = s.substr(offset) + s.substr(0, offset);
            int odd, even;
            if (b % 2)
            {
                odd = solve_1625(1, ns, a);
                even = solve_1625(0, ns, a);
            }
            else
            {
                odd = solve_1625(1, ns, a);
                even = 0;
            }
            get_1625(odd, even, ns);
            if (ans.empty() || ans > ns)
            {
                ans = ns;
            }
        }
        return ans;
    }

    //1627. Graph Connectivity With Threshold
    vector<bool> areConnected(int n, int threshold, vector<vector<int>>& queries) {
        vector<int> parents(n + 1);
        for (int i = 1; i <= n; i++)
            parents[i] = i;
        vector<bool> visited(n + 1);
        
        function<int(vector<int>&, int)> find = [&](vector<int> & parents, int x) {
            if (parents[x] != x)
                parents[x] = find(parents, parents[x]);
            return parents[x];
        };
        for (int i = threshold + 1; i < n; i++) {
            if (visited[i])
                continue;
            visited[i] = true;
            int p1 = find(parents, i);
            for (int j = 2 * i; j <= n; j += i) {
                int p2 = find(parents, j);
                parents[p2] = p1;
            }
        }

        vector<bool> ans;
        for (auto& q : queries)
            ans.push_back(find(parents, q[0]) == find(parents, q[1]));
        return ans;
    }

    //1646. Get Maximum in Generated Array
    int getMaximumGenerated(int n) {
        if (n == 0) return 0;
        if (n == 1) return 1;
        vector<int> a(n + 1);
        a[0] = 0;
        a[1] = 1;
        for (int i = 2; i <= n; ++i)
        {
            if (i & 1)
            {
                a[i] = a[i / 2] + a[i / 2 + 1];
            }
            else
            {
                a[i] = a[i / 2];
            }
        }
        return *max_element(a.begin(), a.end());
    }

    //1647. Minimum Deletions to Make Character Frequencies Unique
    int minDeletions(string s) {
        vector<int> a(26);
        for (auto& c : s) a[c - 'a'] ++;
        sort(a.begin(), a.end());
        int n = a.size();
        int cur = INT_MAX;
        int ans = 0;
        for (int i = n - 1; i >= 0; --i)
        {
            if (a[i] == 0) break;
            if (a[i] < cur)
            {
                cur = a[i] - 1;
            }
            else
            {
                ans += a[i] - cur;
                cur = max(cur - 1, 0);
            }
        }
        return ans;
    }
/*
* //1648. Sell Diminishing-Valued Colored Balls
    int const mod = 1e9 + 7;
long long fastpow(long long p, long long v)
{
    long long ans = 1;
    while (v)
    {
        if (v & 1)
        {
            ans = (ans * p) % mod;
        }
        v >>= 1;
        p = (p * p) % mod;
    }
    return ans;
}

long long inv2 = fastpow(2, mod - 2);

class Solution {
public:
    // add from prev to value
    long long merge(long long value, long long cnt, long long prev)
    {
        return (cnt * (value + prev) % mod) * (value - prev + 1) % mod * inv2 % mod;
    }
    int mergeCnt(long long value, long long cnt, long long prev)
    {
        return (value - prev + 1) * cnt % mod;
    }

    int maxProfit(vector<int>& a, int p) {
        map<int, int> cnt;
        cnt[0] = 0;
        for (auto& e : a) cnt[e] ++;
        long long ans = 0;
        while (p)
        {
            auto last = prev(cnt.end());
            int lastv = last->first;
            int lastc = last->second;
            auto llast = prev(last);
            long long mcnt = mergeCnt(lastv, lastc, llast->first + 1);
            if (mcnt <= p)
            {
                ans = (ans + merge(lastv, lastc, llast->first + 1)) % mod;
                cnt.erase(lastv);
                cnt[llast->first] += lastc;
                p -= mcnt;
            }
            else
            {
                if (lastc >= p)
                {
                    ans = (ans + (long long)p * lastv) % mod;
                }
                else
                {
                    int l = llast->first, r = lastv;
                    while (l < r)
                    {
                        int m = (l + r) / 2;
                        auto mcnt = mergeCnt(lastv, lastc, m);
                        if (mcnt <= p)
                        {
                            r = m;
                        }
                        else
                        {
                            l = m + 1;
                        }
                    }
                    auto mv = merge(lastv, lastc, l);
                    auto mc = mergeCnt(lastv, lastc, l);
                    ans = (ans + mv) % mod;
                    auto left = p - mc;
                    ans = (ans + (long long)(l - 1) * left) % mod;
                }
                break;
            }
        }
        return ans;
    }
};
*/
    //1649. Create Sorted Array through Instructions
    int createSortedArray(vector<int>& ins) {
        int ans = 0;
        int mxn = 1e5;
        BIT bt(mxn + 1);
        int sum = 0;
        int const mod = 1e9 + 7;
        for (int e : ins)
        {
            int mi = bt.query(e - 1);
            int v = min(mi, sum - bt.query(e));
            bt.add(e, 1);
            sum++;
            ans = (ans + v) % mod;
        }
        return ans;
    }

    //1657. Determine if Two Strings Are Close
    bool closeStrings(string word1, string word2) {
        vector<int> a(26), b(26);
        for (auto& c : word1) a[c - 'a'] ++;
        for (auto& c : word2) b[c - 'a'] ++;
        for (int i = 0; i < 26; ++i)
        {
            if (a[i] && !b[i]) return false;
            if (!a[i] && b[i]) return false;
        }
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        int ans = true;
        for (int i = 0; i < 26; ++i)
        {
            if (a[i] != b[i]) ans = false;
        }
        return ans;
    }

    //1658. Minimum Operations to Reduce X to Zero
    int minOperations(vector<int>& a, int x) {
        int n = a.size();
        int sum = 0;
        int left = n - 1;
        int ans = n + 1;
        for (int i = 0; i < n; ++i)
        {
            sum += a[i];
            if (sum == x)
            {
                ans = i + 1;
            }
            else if (sum > x)
            {
                left = i;
                break;
            }
        }
        int right = n - 1;
        for (int i = left; i >= 0; --i)
        {
            sum -= a[i];
            while (right >= i && a[right] + sum <= x)
            {
                sum += a[right];
                if (sum == x)
                {
                    ans = min(ans, i + n - right);
                }
                --right;
            }
        }
        return ans == n + 1 ? -1 : ans;
    }
};

int main()
{
    Solution sol;
    cout << sol.maximum69Number(9669) << endl;
    return 0;
}
