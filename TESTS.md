# TESTS.md

## Manual Test Cases

### 1. Empty input
**Input:**
[press Enter]

**Expected:**
- shell prints a new prompt
- no command runs
- no error message

---

### 2. Spaces-only input
**Input:**
`   `

**Expected:**
- shell prints a new prompt
- no command runs
- no error message

---

### 3. Simple external command
**Input:**
`echo hello`

**Expected output:**
`hello`

---

### 4. Input normalization and tokenization
**Input:**
`   echo     hello      world   `

**Expected output:**
`hello world`

---

### 5. Built-in `cd`
**Input sequence:**
`pwd`
`cd ..`
`pwd`

**Expected:**
- first `pwd` prints current directory
- second `pwd` prints the parent directory

---

### 6. Built-in `exit`
**Input:**
`exit`

**Expected:**
- shell terminates cleanly

---

### 7. Invalid command
**Input:**
`not_a_real_command`

**Expected:**
- shell prints an error
- shell does not crash
- shell returns to prompt

---

### 8. Output redirection
**Input sequence:**
`echo hello > out.txt`
`cat out.txt`

**Expected:**
- first command prints nothing to terminal
- `out.txt` is created
- second command prints `hello`

---

### 9. Input redirection
**Preparation outside the shell:**
`echo hello world > in.txt`

**Input to shell:**
`cat < in.txt`

**Expected output:**
`hello world`

---

### 10. Simple pipe
**Input:**
`echo hello | wc`

**Expected:**
- command runs without crashing
- `wc` prints counts for one line, one word, and the correct byte count
- exact spacing may vary depending on system

---

### 11. Missing left side of pipe
**Input:**
`| wc`

**Expected:**
- shell prints pipe syntax error
- shell does not terminate
- shell returns to prompt

---

### 12. Missing right side of pipe
**Input:**
`ls |`

**Expected:**
- shell prints pipe syntax error
- shell does not terminate
- shell returns to prompt

---

### 13. More than one pipe
**Input:**
`ls | wc | cat`

**Expected:**
- shell prints unsupported pipe syntax error
- shell does not terminate
- shell returns to prompt