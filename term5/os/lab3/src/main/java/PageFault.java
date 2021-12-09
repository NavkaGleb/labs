import java.util.*;

public class PageFault
{
  private static int position = 0;

  public static void replacePage(Vector<Page> memory, int virtualPageCount, int replacePage, ControlPanel controlPanel)
  {

    /*
    FIFO Algorithm
    int count = 0;
    int oldestPage = -1;
    int oldestTime = 0;
    int firstPage = -1;
    int map_count = 0;
    boolean mapped = false;

    while (!mapped || count != virtualPageCount)
    {
      Page page = memory.get(count);

      if (page.physical != -1)
      {
        if (firstPage == -1)
        {
          firstPage = count;
        }

        if (oldestTime < page.inMemTime)
        {
          oldestTime = page.inMemTime;
          oldestPage = count;
          mapped = true;
        }
      }

      count++;

      if (count == virtualPageCount)
      {
        mapped = true;
      }
    }

    if (oldestPage == -1)
    {
      oldestPage = firstPage;
    }*/

    // Clock algorithm
    int foundPageIndex;

    while (true)
    {
      var page = memory.get(position);

      if (page.physical != -1)
      {
        if (page.R == 1)
        {
          page.R = 0;
        }
        else
        {
          foundPageIndex = position;
          break;
        }
      }

      position = (position + 1) % virtualPageCount;
    }

    Page page = memory.get(foundPageIndex);
    Page nextPage = memory.get(replacePage);

    nextPage.physical = page.physical;

    controlPanel.removePhysicalPage(foundPageIndex);
    controlPanel.addPhysicalPage(nextPage.physical, replacePage);

    page.inMemTime = 0;
    page.lastTouchTime = 0;
    page.R = 0;
    page.M = 0;
    page.physical = -1;
  }
}
