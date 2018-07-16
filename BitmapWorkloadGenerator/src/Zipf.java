import java.util.Random;

/**
 * Objects of this represent a number generator based on the Zipf distribution.
 * A rank_id is returned with a probability of (1/rank_id^{skew}) / denom, where
 * denom = \sum_{i=1}{N}{1/N^{skew}}
 *
 * @author David
 */
public class Zipf
{
    private int size; /** size of the distribution (i.e., N) */
    private double skew; /** skew of the zipf distribution */
    private double denom; /** denominator of the zipf distribution */
    private Random rnd;

    public Zipf(int size, double skew) {
        this.rnd = new Random(System.currentTimeMillis());
        this.size = size;
        this.skew = skew;
        this.denom = 0;
        for(int i = 1; i <= size; i++) {
            this.denom += (1 / Math.pow(i, this.skew));
        }
    }

    /**
     * The frequency of returned rank ids are follows Zipf distribution
     * with the current skew.
     *
     * @return a rank id (where rank > 0)
     */
    public int next() {
        int rank_id;
        double frequency;
        do {
            rank_id = rnd.nextInt(size); //choose a rank from the range [0,size-1]
            frequency = (1.0d / Math.pow(rank_id + 1, this.skew)) / this.denom;
        } while(rnd.nextDouble() >= frequency);
        return rank_id;
    }

    /**
     * This method returns a probability that the given rank id occurs.
     *
     * @param rank A zero-based rank
     * @return the probability of occurrence of the given rank_id
     */
    public double getProbability(int rank_id) {
        return (1.0d / Math.pow(rank_id + 1, this.skew)) / this.denom;
    }
}
