#ifndef PADXYSCALES_H_
#define PADXYSCALES_H_

#define NUMBER_OF_SCALES 2

const uint8_t xyScale[NUMBER_OF_SCALES][128] = {

    //easier to put scales directly to flash than in progmem
    //we would have to extract each byte from progmem which results in slower access

    //wide middle
    {
0,
10,
16,
21,
24,
27,
29,
31,
33,
35,
36,
38,
39,
40,
41,
42,
43,
44,
45,
46,
46,
47,
48,
48,
49,
50,
50,
51,
51,
52,
52,
53,
53,
54,
54,
55,
55,
55,
56,
56,
57,
57,
57,
58,
58,
58,
59,
59,
59,
60,
60,
60,
60,
61,
61,
61,
62,
62,
62,
62,
63,
63,
63,
63,
64,
64,
64,
64,
65,
65,
65,
65,
66,
66,
66,
67,
67,
67,
67,
68,
68,
68,
69,
69,
69,
70,
70,
70,
71,
71,
72,
72,
72,
73,
73,
74,
74,
75,
75,
76,
76,
77,
77,
78,
79,
79,
80,
81,
81,
82,
83,
84,
85,
86,
87,
88,
89,
91,
92,
94,
96,
98,
100,
103,
106,
111,
117,
127
    },

    {
    //wide ends
0,
0,
0,
0,
1,
1,
1,
1,
2,
2,
2,
3,
3,
3,
3,
4,
4,
4,
5,
5,
5,
6,
6,
6,
7,
7,
8,
8,
8,
9,
9,
10,
10,
11,
11,
12,
12,
13,
13,
14,
15,
15,
16,
17,
17,
18,
19,
20,
21,
22,
23,
24,
25,
27,
28,
30,
32,
34,
36,
39,
42,
47,
53,
63,
64,
74,
80,
85,
88,
91,
93,
95,
97,
99,
100,
102,
103,
104,
105,
106,
107,
108,
109,
110,
110,
111,
112,
112,
113,
114,
114,
115,
115,
116,
116,
117,
117,
118,
118,
119,
119,
119,
120,
120,
121,
121,
121,
122,
122,
122,
123,
123,
123,
124,
124,
124,
124,
125,
125,
125,
126,
126,
126,
126,
127,
127,
127,
127
    }

};



#endif /* PADXYSCALES_H_ */