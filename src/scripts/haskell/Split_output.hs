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
    fmap (BS.split $ (fromIntegral( ord in_c))) (BS.readFile fname)


--extracts named data parameter from a bytestring holding a dataset
--should be called with the : appended to the data name
extract_data_name :: String -> [BS.ByteString] -> [BS.ByteString]
extract_data_name sd_name split_out=
        map (BS.tail.BS.init) split_v
        where 
              d_name = CH.pack (sd_name ++ ":")
              split_v = map (BS.drop (BS.length d_name)) (mapMaybe m_func split_out)
              m_func= DL.find(BS.isPrefixOf d_name).CH.lines

extract_single_data :: BS.ByteString -> BS.ByteString -> Maybe BS.ByteString
extract_single_data dname dstr = 
        fmap (BS.tail.BS.init.BS.drop (BS.length dname)) ext_d
        where
            ext_d = get_f dstr
            get_f = DL.find(BS.isPrefixOf dname).CH.lines


extract_data :: String -> String -> IO [BS.ByteString]
extract_data dat_name fname =
        fmap (extract_data_name dat_name)  (split_byte_file '&' fname)

--implement more specific parsing algorithms with ParSec
