module Get_top_scores
    where

import Split_output
import qualified Data.List as DL
import qualified Data.Maybe as DM
import qualified Data.ByteString as BS
import qualified Data.ByteString.Char8 as CH
import Numeric as NU
get_top_scores :: Int -> [BS.ByteString] -> [BS.ByteString]
get_top_scores num instrs =
        take num sort_array
        where
            sort_array = DL.sortBy (\x y -> compare (ext_str y) (ext_str x)) instrs

            rstr :: BS.ByteString -> Maybe BS.ByteString
            rstr inv = extract_single_data (CH.pack "score:") inv

            ext_str :: BS.ByteString -> Float
            ext_str instr =
                read (CH.unpack (DM.fromMaybe (CH.pack "-1") mstr))
                where
                    mstr = rstr instr

get_top_nums :: Int -> [BS.ByteString] -> [Float]
get_top_nums num instrs =
        --take num (DL.sortBy (\x y -> compare (NU.readDec (ext_str y))  (NU.readDec (ext_str x))) instrs)
        take num sort_array
        where
            sort_array = DL.sortBy (\x y -> compare y x) (map ext_str instrs)

            rstr :: BS.ByteString -> Maybe BS.ByteString
            rstr inv = extract_single_data (CH.pack "score:") inv

            ext_str :: BS.ByteString -> Float
            ext_str instr =
                read (CH.unpack (DM.fromMaybe (CH.pack "-1") mstr))
                where
                    mstr = rstr instr
