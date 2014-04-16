module Split_output
    where
import Data.Char
import Data.Maybe
import qualified Data.List as DL
import qualified Data.ByteString as BS
import qualified Data.ByteString.Char8 as CH
--Splits the lilac output file into strings representing that data from each run
split_byte_file :: Char -> String -> IO [BS.ByteString]
split_byte_file in_c fname=
    do
        fmap (BS.split $ (fromIntegral( ord in_c))) (BS.readFile fname)


--extracts named data parameter from a bytestring holding a dataset
--should be called with the : appended to the data name
extract_data_name :: BS.ByteString -> [BS.ByteString] -> [BS.ByteString]
extract_data_name d_name split_out=
    map (BS.tail.BS.init) split_v
    where 
        split_v = map (BS.drop (BS.length d_name)) (catMaybes (map m_func split_out))
        m_func= DL.find(BS.isPrefixOf(d_name)).CH.lines


extract_data :: String -> String -> IO [BS.ByteString]
extract_data dat_name fname =
    fmap (extract_data_name (CH.pack(dat_name++[':'])))  (split_byte_file '&' fname)

--implement more specific parsing algorithms with ParSec
--sum_reals :: IO [BS.ByteString] -> Float

