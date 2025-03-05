from subprocess import run, Popen, PIPE
from sys import executable

num_test = 5
is_ok = True
for i in range(1, num_test + 1):
    str_data =  "tests/end_to_end/" + str(i) + ".dat"
    file_in = open(str_data, "r")
    str_ans = "tests/end_to_end/" + str(i) + ".ans"

    ans = []
    for j in open(str_ans):
        ans.append(int(j.strip()))
	
    result = run(["build/src/main"], capture_output = True, encoding='cp866', stdin=file_in)
    print("Test: ", i)

    res = list(map(int, result.stdout.split()))

    is_ok &= (res == ans)
    if res == ans:
        print("OK")
    else:
        print("ERROR\nExpect:", ans, "\nGive:  ", res)
    print("-------------------------------------------------")

if is_ok:
	print("TESTS PASSED")
else:
	print("TESTS FAILED")