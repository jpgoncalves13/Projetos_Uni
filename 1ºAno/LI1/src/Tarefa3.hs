{- | 
 -   O principal objetivo desta Tarefa é converter um labirinto numa forma mais adequada para leitura, criando a função compactMaze.
    
 -   Começámos por definir a função chooseIndCorr que dado uma lista de corredores e um corredor, escolhe a posição onde um corredor se encontra, por exemplo se se encontrar na primeira posição retorna o valor 0.
     Seguidamente, definimos a função convertToInstruct que transforma um corredor numa Instruction e a função instructToInstruct que dado uma lista de pares Int e Piece, se para dois elementos consecutivos as Pieces são iguais, então vai se transformar num elemento em que a primeira componente vai ser composta pela Piece do primeiro par somado com um, e assim consecutivamente até percorrer a lista toda.
     Se os dois elementos consecutivos não tiverem as Pieces iguais, então esses elementos vão ficar iguais.
     Por fim, fizemos a função compactMaze que transforma um labirinto numa lista de instruções.

 -   Concluindo, o principal resultado obtido nesta tarefa foi a compactação do labirinto para uma forma mais fácil de se ler.
                                                                                                                         -}



module Tarefa3 where 

import Types 
import Data.List


-- | Creates Intructions given a Maze   

-- ^The function verify if the last element belong to the rest of the list, and to this recursively     

compactMaze :: Maze -> Instructions              
compactMaze []     = []                                                     
compactMaze l | elem (last l) (init l) = compactMaze (init l) ++  [Repeat (chooseIndCorr (init l) (last l))] 
              | otherwise = compactMaze (init l) ++ [convertToInstruct (last l)] 




-- | Calculates given a list of corridors and a corridor from that list, the position in which it is located, where the runner at the top of the list is in position 0 

chooseIndCorr :: [Corridor] -> Corridor -> Int  
chooseIndCorr (x:xs) a | a == x    = 0  
                       | otherwise = 1 + (chooseIndCorr xs a) 



-- | Given a corridor this function converts it to an Instruction
convertToInstruct :: Corridor -> Instruction 
convertToInstruct c = Instruct ( instructToInstruct (map (\x -> (1,x)) c)) 
 


-- | Given a list of pairs converts it into a list where the consecutive elements are equal, that is, where the Piece in the second element of the pair are the same in consecutives pairs of the list, come together 

-- ^If the consecutive elements are equal we add on one to the first element of the pair where the function retains the first pair with the changes otherwise the first pair go to the head of the final list and to this recursively
instructToInstruct :: [(Int, Piece)] -> [(Int, Piece)] 
instructToInstruct ((a,b):[])  = [(a,b)]
instructToInstruct ((a,b):(c,d):xs) | b == d    = instructToInstruct ((a+1,b):xs) 
                                    | otherwise = (a,b) : instructToInstruct ((c,d):xs) 