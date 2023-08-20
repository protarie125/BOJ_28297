#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <tuple>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;

using ll = long long;
using vb = vector<bool>;
using vvb = vector<vb>;
using vi = vector<int>;
using vvi = vector<vi>;
using vl = vector<ll>;
using vvl = vector<vl>;
using pii = pair<int, int>;
using vii = vector<pii>;
using pll = pair<ll, ll>;
using vll = vector<pll>;

const double Pi = acos(-1);

class Circle {
public:
	ll x;
	ll y;
	ll r;
};

using vC = vector<Circle>;

vC circles;
vi root;

double dist(const Circle& a, const Circle& b) {
	const auto dx = a.x - b.x;
	const auto dy = a.y - b.y;

	return sqrt(pow(dx, 2) + pow(dy, 2));
}

int par(int x) {
	if (root[x] == x) {
		return x;
	}

	return root[x] = par(root[x]);
}

void joint(int i, int j) {
	i = par(i);
	j = par(j);

	if (i == j) {
		return;
	}

	root[min(i, j)] = max(i, j);
}

double getBeltLength(const Circle& a, const Circle& b) {
	Circle c1, c2;
	if (a.r < b.r) {
		c1 = b;
		c2 = a;
	}
	else {
		c1 = a;
		c2 = b;
	}

	const auto d = dist(c1, c2);
	const auto h = sqrt(pow(d, 2) - pow(c1.r - c2.r, 2));
	const auto th = acos((c1.r - c2.r) / d);

	const auto l1 = c1.r * 2 * (Pi - th);
	const auto l2 = c2.r * 2 * th;

	return 2 * h + l1 + l2;
}

int main() {
	ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int n;
	cin >> n;

	circles = vC(n);
	for (auto&& c : circles) {
		cin >> c.x >> c.y >> c.r;
	}

	// disjoint union
	root = vi(n);
	for (auto i = 0; i < n; ++i) {
		root[i] = i;
	}

	for (auto i = 0; i < n; ++i) {
		const auto& c1 = circles[i];

		for (auto j = i + 1; j < n; ++j) {
			const auto& c2 = circles[j];

			const auto d = dist(c1, c2);
			if (d <= c1.r + c2.r) {
				joint(i, j);
			}
		}
	}

	// MST
	vector<tuple<double, int, int>> edges;
	for (auto i = 0; i < n; ++i) {
		const auto& c1 = circles[i];

		for (auto j = 0; j < n; ++j) {
			if (i == j || root[i] == root[j]) {
				continue;
			}

			const auto& c2 = circles[j];

			edges.push_back({ getBeltLength(c1, c2), i, j });
		}
	}

	sort(edges.begin(), edges.end());

	auto ans = double{ 0 };
	for (const auto& e : edges) {
		const auto& [d, i, j] = e;

		if (par(i) != par(j)) {
			ans += d;
			joint(i, j);
		}
	}

	cout << fixed << setprecision(8) << ans;

	return 0;
}