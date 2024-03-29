#!/usr/bin/python3

# Target steps: 327 (1 db)
#    0  990 5870  987    0 
#  990 6820 7819 6846  987 
# 5851 7819 7796 7819 5886 
# 1005 6827 7819 6836  984 
#    0 1005 5867  984    0 
# Result: 97807

# Target steps: 589 (2 db)
#    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
#    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
#    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
#    0    0    0    0    0    0  990 5870  987    0    0    0    0    0    0 
#    0    0    0    0    0  990 6820 7819 6846  987    0    0    0    0    0 
#    0    0    0    0  990 6820 7819 7796 7819 6846  987    0    0    0    0 
#    0    0    0  990 6820 7819 7796 7819 7796 7819 6846  987    0    0    0 
#    0    0    0 5851 7819 7796 7819 7796 7819 7796 7819 5886    0    0    0 
#    0    0    0 1005 6827 7819 7796 7819 7796 7819 6836  984    0    0    0 
#    0    0    0    0 1005 6827 7819 7796 7819 6836  984    0    0    0    0 
#    0    0    0    0    0 1005 6827 7819 6836  984    0    0    0    0    0 
#    0    0    0    0    0    0 1005 5867  984    0    0    0    0    0    0 
#    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
#    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
#    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
# Result: 316593

# Target steps: 851 (3 db)
#    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
#    0    0    0    0    0    0  990 5870  987    0    0    0    0    0    0 
#    0    0    0    0    0  990 6820 7819 6846  987    0    0    0    0    0 
#    0    0    0    0  990 6820 7819 7796 7819 6846  987    0    0    0    0 
#    0    0    0  990 6820 7819 7796 7819 7796 7819 6846  987    0    0    0 
#    0    0  990 6820 7819 7796 7819 7796 7819 7796 7819 6846  987    0    0 
#    0  990 6820 7819 7796 7819 7796 7819 7796 7819 7796 7819 6846  987    0 
#    0 5851 7819 7796 7819 7796 7819 7796 7819 7796 7819 7796 7819 5886    0 
#    0 1005 6827 7819 7796 7819 7796 7819 7796 7819 7796 7819 6836  984    0 
#    0    0 1005 6827 7819 7796 7819 7796 7819 7796 7819 6836  984    0    0 
#    0    0    0 1005 6827 7819 7796 7819 7796 7819 6836  984    0    0    0 
#    0    0    0    0 1005 6827 7819 7796 7819 6836  984    0    0    0    0 
#    0    0    0    0    0 1005 6827 7819 6836  984    0    0    0    0    0 
#    0    0    0    0    0    0 1005 5867  984    0    0    0    0    0    0 
#    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
# Result: 660299

# Target steps: 1113 (4 db)
#    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
#    0    0    0    0    0    0    0    0  990 5870  987    0    0    0    0    0    0    0    0 
#    0    0    0    0    0    0    0  990 6820 7819 6846  987    0    0    0    0    0    0    0 
#    0    0    0    0    0    0  990 6820 7819 7796 7819 6846  987    0    0    0    0    0    0 
#    0    0    0    0    0  990 6820 7819 7796 7819 7796 7819 6846  987    0    0    0    0    0 
#    0    0    0    0  990 6820 7819 7796 7819 7796 7819 7796 7819 6846  987    0    0    0    0 
#    0    0    0  990 6820 7819 7796 7819 7796 7819 7796 7819 7796 7819 6846  987    0    0    0 
#    0    0  990 6820 7819 7796 7819 7796 7819 7796 7819 7796 7819 7796 7819 6846  987    0    0 
#    0  990 6820 7819 7796 7819 7796 7819 7796 7819 7796 7819 7796 7819 7796 7819 6846  987    0 
#    0 5851 7819 7796 7819 7796 7819 7796 7819 7796 7819 7796 7819 7796 7819 7796 7819 5886    0 
#    0 1005 6827 7819 7796 7819 7796 7819 7796 7819 7796 7819 7796 7819 7796 7819 6836  984    0 
#    0    0 1005 6827 7819 7796 7819 7796 7819 7796 7819 7796 7819 7796 7819 6836  984    0    0 
#    0    0    0 1005 6827 7819 7796 7819 7796 7819 7796 7819 7796 7819 6836  984    0    0    0 
#    0    0    0    0 1005 6827 7819 7796 7819 7796 7819 7796 7819 6836  984    0    0    0    0 
#    0    0    0    0    0 1005 6827 7819 7796 7819 7796 7819 6836  984    0    0    0    0    0 
#    0    0    0    0    0    0 1005 6827 7819 7796 7819 6836  984    0    0    0    0    0    0 
#    0    0    0    0    0    0    0 1005 6827 7819 6836  984    0    0    0    0    0    0    0 
#    0    0    0    0    0    0    0    0 1005 5867  984    0    0    0    0    0    0    0    0 
#    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
# Result: 1128925


spikes = [5870, 5851, 5867, 5886]
outer_shell = [990, 987, 1005, 984]
inner_shell = [6820, 6827, 6836, 6846]
center_fill = 7796
outer_fill = 7819

# 26501365 // 262

dblocks = 26501365 // 262

result = sum(spikes)
result += sum([o * 2 * dblocks for o in outer_shell])
result += sum([i * (2 * dblocks -1) for i in inner_shell])

# inner: 1, 8, 16, 24
# outer: 4, 12, 20, 28


# db:         2        3        4
# inner: 1, 0 + 8*1, 0 + 8*2, 0 + 8*3
# outer: 4, 4 + 8*1, 4 + 8*2, 4 + 8*3

# inner
# 8*1 + 8*2 + 8*3
# 8 * (1 + 2 + 3)
# S = n(a + l)/2

# outer
# 4 + 4 + 8*1 + 4 + 8*2 + 4 + 8*3
# 4 + 4 + 4 + 4 + 8*1 + 8*2 + 8*3

dbc = 4 * (dblocks - 1) * dblocks

inner_count = 1 + dbc
outer_count = 4 * dblocks + dbc

result += inner_count * center_fill
result += outer_count * outer_fill


print (result)
