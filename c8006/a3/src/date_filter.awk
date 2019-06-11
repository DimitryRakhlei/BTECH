
BEGIN {
    SCAN_BACK = 1
    date_format="+%b %e %H:%M:%S"
    "date -d " "'now "SCAN_BACK" minutes ago' " "'"date_format"'"  | getline scope
    print today
}

{
    unformatted = $1 " " $2 " " $3
    "date -d '" unformatted "' " "'" date_format "'" | getline access_date
    if (access_date > scope) {
        print $0
    }
}
