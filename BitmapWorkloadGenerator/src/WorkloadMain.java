/**
 * Generates bitmaps and queries for bitmap experiments.
 */
public class WorkloadMain {
    /*********************
     * Output File Names *
     *********************/

    /** Name of the ouput file containing the bitmaps. */
    public static final String bitmap_out = "bitmap_out.txt";

    /** Name of the ouput file containing the grey code-ordered bitmaps. */
    public static final String bitmap_out_gc = "bitmap_out_gc.txt";

    /** Name of the output file containing the queries. */
    public static final String query_out = "query_out.txt";

    /*****************
     * Bitmap Fields *
     *****************/

    /** Number of attributes. */
    public static final int num_attributes = 10000;

    /** Cardinality (number of bins per attribute). */
    public static final int cardinality = 10;

    /** Number of rows. */
    public static final int num_rows = 1048576;

    /**
     * Skew for which bins are chosen to be favored with a 1.
     * A value of 0 implies uniform distribution.
     * A value of "infinity" implies the first rank always being picked.
     */
    public static final int data_skew = 2;

    /****************
     * Query Fields *
     ****************/

    /**
     * The query mode; must be one of:
     *     QueryGenerator.POINT_ONLY
     *     QueryGenerator.RANGE_ONLY
     *     QueryGenerator.MIXED_MODE
     */
    public static final int mode = QueryGenerator.MIXED_MODE;

    /** Number of queries to generate. */
    public static final int num_queries = 1000;

    /**
     * The ratio of queries to be point queries.
     * Thus, a value of 0.7 implies queries containing 70% point queries.
     * Only applicable in MIXED_MODE.
     */
    public static final double query_ratio = 0.7;

    /**
     * Skew for which attributes and bins are to be queried.
     * For more information, see explanation of {@code data_skew} above.
     */
    public static final int qry_skew_att = 2;
    public static final int qry_skew_bin = 2;

    public static void main(String[] args) {
        // Bitmaps
        System.out.println("Generating Bitmaps");
        DataGenerator data = new DataGenerator(num_attributes, cardinality, data_skew, num_rows);
        data.writeFile(bitmap_out);

        // Grey Code-Ordered Bitmaps
        System.out.println("Generating Grey Code-Ordered Bitmaps");
        GreyCodeSorter sorter = new GreyCodeSorter(bitmap_out);
        sorter.writeFile(bitmap_out_gc);

        // Queries
        System.out.println("Generating Queries");
        QueryGenerator query = new QueryGenerator(num_attributes, cardinality, num_queries, qry_skew_att, qry_skew_bin);
        query.writeFile(mode, query_ratio, query_out);
    }
}
