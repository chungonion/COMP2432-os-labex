#!/bin/bash

# first count the argument list, then obtain nos of rounds
# get all those files and the arguments to formulate the result stat

# echo $@
teamcount=0
roundcount=0
gamePerRound=0
teamname=()
teamresult=()
teamWin=()
teamTie=()
teamLost=()
teamGF=()
teamGA=()
teamGD=()
teammatchcount=()
filename=0

getTeamIndex (){
  local i #Local variable!!!!
  for i in ${!teamname[@]};do
    #   echo ${teamname[$i]}
     if [[ "${teamname[$i]}" = "$1" ]]; then
         return ${i}
     fi
    #TODO!
  done
  return 255 # Return not found
}

checkGameRound (){
    temp=$((teamcount%2))
    if [ $temp -eq 1 ];then
        gamePerRound=$(((teamcount-1)/2))
    else
        gamePerRound=$((teamcount/2))
    fi
}

checkMissingGame (){
    for ((i = 0; i < teamcount; i++)); do
        :
    done
}

displayResult (){
    local teamOrder=()
    local i
    local j

    #Sorting!!

    #init
    for ((i = 0; i < teamcount; i++)); do
        # bash arrays are 0-indexed
        teamOrder[$i]=$i
    done

    # TODO selection sort!!!
    for((i=0;i<teamcount;i++));do
        tempLargest=$i
        for((j=teamcount-1;j>i;j--));do
            if [ $((teamresult[teamOrder[j]])) -gt $((teamresult[teamOrder[tempLargest]])) ];then
                tempLargest=$j
            fi
            # echo J= $j
            # echo LArgest= $tempLargest
        done
        temp=$((teamOrder[i]))
        teamOrder[$i]=${teamOrder[$tempLargest]}
        teamOrder[$tempLargest]=$temp
        # echo ${teamOrder[@]}

    done
    # echo ${teamOrder[@]}

    printf "%-8s%-12s%-5s%-5s%-5s%-5s%-6s%-6s%-6s%-9s\n" "Rank" "Team" "P" "W" "D" "L" "GF" "GA" "GD" "Points"

    for((i=0;i<${#teamOrder[@]};i++));do
        gamePlayed=$((teamWin[teamOrder[i]]+teamTie[teamOrder[i]]+teamLost[teamOrder[i]]))
        gameMissed=$((roundcount-gamePlayed-2))
        if [ $gameMissed -gt 0 ];then
            printf "%-8s%-12s%-5s%-5s%-5s%-5s%-6s%-6s%-6s%-9s\n" \
            $((i+1)) ${teamname[${teamOrder[$i]}]} "$((gamePlayed+gameMissed))($gamePlayed)" $((teamWin[teamOrder[i]])) "$((teamTie[teamOrder[i]]+gameMissed))($((teamTie[teamOrder[i]])))" $((teamLost[teamOrder[i]]))\
            $((teamGF[teamOrder[i]])) $((teamGA[teamOrder[i]])) $((teamGF[teamOrder[i]]-teamGA[teamOrder[i]])) "$((teamresult[teamOrder[i]]+gameMissed))($((teamresult[teamOrder[i]])))"
        else
            printf "%-8s%-12s%-5s%-5s%-5s%-5s%-6s%-6s%-6s%-9s\n" \
            $((i+1)) ${teamname[${teamOrder[$i]}]} $gamePlayed $((teamWin[teamOrder[i]])) $((teamTie[teamOrder[i]])) $((teamLost[teamOrder[i]]))\
            $((teamGF[teamOrder[i]])) $((teamGA[teamOrder[i]])) $((teamGF[teamOrder[i]]-teamGA[teamOrder[i]])) $((teamresult[teamOrder[i]]))
        fi
    done
   #TODO display the result, print that accordingly.
  #statements
}

for i in ${@:1:${#}-1};do

    teamname[teamcount]=$i # pre-increment teamcount, add it to array
    teamGA[teamcount]=0
    teamGF[teamcount]=0
    teamWin[teamcount]=0
    teamTie[teamcount]=0
    teamLost[teamcount]=0
    teamresult[teamcount]=0
    teamcount=$((teamcount+1))

done
roundcount=$((teamcount*2))
filename=${@: -1}
echo $filename

echo team: $teamcount
checkGameRound

if [ $roundcount -ne $(($teamcount*2)) ];then #integer comparision
  echo Not Sufficient rounds of competition! # Reserve for counting games
fi

for ((i=1; i <= $roundcount;i++));do
  if [ ! -f $filename$i ];then
    echo "Error! The $filename$i file is not found!"
    #   exit
  else
    line=`cat $filename$i`
    # exec < $filename
    IFS=' ' read -r -a array <<< "$line"
    # input of readline
    # echo ${array[@]}
    # for j in ${array[@]};do
    #   echo $j
    # done

    # echo "games in this file ${#array[@]}"
    if [ ${#array[@]} -ne $((gamePerRound*4)) ];then #the math is 2 team's name plus 2 teams' goals = 4 params
        echo "Argument error!"
    else : #TODO stuff here
        for ((j=0;j < ${#array[@]};j+=4));do
            getTeamIndex ${array[$j]}
            aindex=$?


            getTeamIndex ${array[$((j+2))]}
            bindex=$?

            if [ $aindex -ne 255 -a $bindex -ne 255 ];then
                agoal=${array[$((j+1))]}
                bgoal=${array[$((j+3))]}
                if [ $agoal -gt $bgoal ];then
                  teamWin[aindex]=$((teamWin[aindex]+1))
                  teamLost[bindex]=$((teamLost[bindex]+1))
                  teamresult[aindex]=$((teamresult[aindex]+3))
                #   echo ${teamresult[aindex]}

                  elif [ $agoal -lt $bgoal ];then
                      teamWin[bindex]=$((teamWin[bindex]+1))
                      teamLost[aindex]=$((teamLost[aindex]+1))
                      teamresult[bindex]=$((teamresult[bindex]+3))
                    #   echo ${teamresult[bindex]}
                else
                  teamTie[aindex]=$((teamTie[aindex]+1))
                  teamTie[bindex]=$((teamTie[bindex]+1))
                  teamresult[aindex]=$((teamresult[aindex]+1))
                  teamresult[bindex]=$((teamresult[bindex]+1))
                #   echo TeamA: ${teamresult[aindex]}
                #   echo TeamB: ${teamresult[bindex]}

                fi

                teamGF[aindex]=$((teamGF[aindex]+agoal))
                teamGA[aindex]=$((teamGA[aindex]+bgoal))

                teamGF[bindex]=$((teamGF[bindex]+bgoal))
                teamGA[bindex]=$((teamGA[bindex]+agoal))
            else
                echo "Error 404: One (or more) team(s) are not found!"
            fi
        done
    fi

    # variable like the index of two team, and also needs to calculate the result.

    #find the team name, get the index, and calculate the result
  fi

done
# for i in teams do sth
# for i in rounds do sth blar blar blar
displayResult
# echo Team:$teamcount Round:$roundcount Total Games should be played:$(($teamcount*2)) Game per round: $gamePerRound
# echo Scores : ${teamname[0]} ${teamresult[0]} ${teamGF[0]} ${teamGA[0]} Win:${teamWin[0]} Tie:${teamTie[0]} Lost:${teamLost[0]}
# echo Scores : ${teamname[1]} ${teamresult[1]} ${teamGF[1]} ${teamGA[1]} Win:${teamWin[1]} Tie:${teamTie[1]} Lost:${teamLost[1]}
# echo Scores : ${teamname[2]} ${teamresult[2]} ${teamGF[2]} ${teamGA[2]} Win:${teamWin[2]} Tie:${teamTie[2]} Lost:${teamLost[2]}
# echo Scores : ${teamname[3]} ${teamresult[3]} ${teamGF[3]} ${teamGA[3]} Win:${teamWin[3]} Tie:${teamTie[3]} Lost:${teamLost[3]}
# echo Scores : ${teamname[4]} ${teamresult[4]} ${teamGF[4]} ${teamGA[4]} Win:${teamWin[4]} Tie:${teamTie[4]} Lost:${teamLost[4]}
# # Above is a testing statement
