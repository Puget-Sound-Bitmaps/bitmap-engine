import java.util.Random;

/**
 * An abstract class that provides basic shared structure and methods
 * for experiment generators.
 *
 * @author David
 * @version 1/5/17
 */
public abstract class WorkloadGenerator
{
    protected int[][] rank_bin_map; // for each attribute, we have a rank_id to bin mapping
    protected int num_attributes; // number of attributes
    protected int cardinality; // cardinality (bins per attribute)

    /**
     * Constructs a new generator
     * @param num_attributes
     * @param cardinality
     */
    public WorkloadGenerator(int num_attributes, int cardinality)
    {
        this.num_attributes = num_attributes;
        this.cardinality = cardinality;
        //initialize each attribute's rank_id to a bin map
        this.rank_bin_map = new int[num_attributes][cardinality];
        for (int i = 0; i < num_attributes; i++) {
            for (int j = 0; j < cardinality; j++) {
                this.rank_bin_map[i][j] = j;
            }
            shuffle(this.rank_bin_map[i]);
        }
    }

    /**
     * Subclasses must implement this method. Writes the
     * results to a file with the given name
     * @param file_out
     */
    abstract public void writeFile(String file_out);

    /**
     * Helper method shuffles the elements in random order in the given array
     * @param list A reference to an array to be randomly shuffled
     */
    private void shuffle(int[] list)
    {
        Random rng = new Random(System.currentTimeMillis());
        for (int i = 0; i < list.length; i++) {
            int swap_idx = rng.nextInt(list.length - i) + i;
            int swap_element = list[swap_idx];
            list[swap_idx] = list[i];
            list[i] = swap_element;
        }
    }
}
