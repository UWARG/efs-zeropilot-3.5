while read -r line
do
    # skip comment lines and blank lines
    if [[ $line =~ ^\s*#.*$ ]] || [[ -z $(echo $line | tr -d '[:space:]') ]]; then
        continue
    fi

    # split line at "=" into a key-value pair
    IFS="=" read -r key value <<< "$line"
    
    # strip extra spaces
    key=$(echo $key)
    value=$(echo $value)

    # declare variables
    declare $key="${value//\"/}"
done < $1