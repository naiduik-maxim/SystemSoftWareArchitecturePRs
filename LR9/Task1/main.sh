getent passwd | awk -F: '{
    if ($3 >= 1000 && $3 != 65534) 
        print $1 " (UID=" $3 ") - Regular User"
    else 
        print $1 " (UID=" $3 ")"
}' | head -n 15 